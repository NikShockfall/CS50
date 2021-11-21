import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
	response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
	response.headers["Expires"] = 0
	response.headers["Pragma"] = "no-cache"
	return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set [ pk_ee281eb7ebc649c59abb7f1a0d712ed9 ]
if not os.environ.get("API_KEY"):
	raise RuntimeError("API_KEY not set")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
	"""Show portfolio of stocks"""
	balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
	balance = round(float(balance[0]['cash']), 2)
	net = balance

	try:
		rows = db.execute("SELECT DISTINCT(symbol), SUM(shares) FROM stocks WHERE user = ? GROUP BY symbol", session["user_id"])
	except:
		return render_template("index.html", cash=balance, net=balance)

	# Renaming keys for ease of coding in index.html
	for row in rows:
		quote = lookup(row.get("symbol"))
		row['name'] = quote.get("name")
		row['shares']  = row.pop('SUM(shares)')
		row['price'] = round(quote.get("price"), 2)
		row['total'] = round(row['shares']*quote.get("price"), 2)

		# Adds symbol's total price to net total
		net += row['total']

	return render_template("index.html", stocks=rows, cash=balance, net=net)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
	"""Buy shares of stock"""
	if request.method == "POST":

		# Ensure symbol was submitted
		if not request.form.get("symbol"):
			return apology("please enter ticker/symbol", 400)

		if not request.form.get("shares"):
			return apology("please enter number of shares", 400)

		if not request.form.get("shares").isnumeric():
			return apology("please enter number of shares", 400)

		if float(request.form.get("shares")) % 1 != 0:
			return apology("no fractional shares", 400)

		share = int(request.form.get("shares"))

		# Retrieves data based on symbol
		quote = lookup(request.form.get("symbol"))

		# Ensures that the symbol contains a match
		if not quote:
			return apology("symbol not found", 400)

		cost = float(quote['price'] * share)

		# Query user's balance from database
		balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
		balance = balance[0]["cash"]

		# Ensures that balance is enough to obtain shares
		if cost > balance:
			return apology("Not enough balance", 402)


		# Ensures that the table exists
		try:
			db.execute("SELECT * FROM stocks")
		except:
			db.execute("CREATE TABLE stocks ( id INTEGER, user INTEGER, symbol TEXT NOT NULL, shares INTEGER, price NUMERIC NOT NULL, " +
						" date NUMERIC NOT NULL, time NUMERIC NOT NULL, PRIMARY KEY (id), FOREIGN KEY(user) REFERENCES users(id)  ) ")

		# Tracks purchase into database
		now = datetime.datetime.now()
		db.execute("INSERT INTO stocks (user, symbol, shares, price, date, time) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"], quote['symbol'], share, quote['price'], now.strftime("%Y-%m-%d"), now.strftime("%H:%M:%S"))

		# Updates user's balance
		db.execute("UPDATE users SET cash = ? WHERE id = ?", balance - cost, session["user_id"])

		flash("Bought!")
		return redirect("/")


	return render_template("buy.html")


@app.route("/history")
@login_required
def history():
	"""Show history of transactions"""

	try:
		rows = db.execute("SELECT symbol, shares, price, date, time FROM stocks WHERE user = ?", session["user_id"])
	except:
		return render_template("history.html")

	return render_template("history.html", stocks=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
	"""Log user in"""

	# Forget any user_id
	session.clear()

	# User reached route via POST (as by submitting a form via POST)
	if request.method == "POST":

		# Ensure username was submitted
		if not request.form.get("username"):
			return apology("must provide username", 403)

		# Ensure password was submitted
		elif not request.form.get("password"):
			return apology("must provide password", 403)

		# Query database for username
		rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

		# Ensure username exists and password is correct
		if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
			return apology("invalid username and/or password", 403)

		# Remember which user has logged in
		session["user_id"] = rows[0].get("id")

		flash("Welcome " + request.form.get("username") + "!")
		# Redirect user to home page
		return redirect("/")

	# User reached route via GET (as by clicking a link or via redirect)
	return render_template("login.html")


@app.route("/logout")
def logout():
	"""Log user out"""

	# Forget any user_id
	session.clear()

	# Redirect user to login form
	return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

	# User reached route via POST (as by submitting a form via POST)
	if request.method == "POST":
		# Ensure symbol was submitted
		if not request.form.get("symbol"):
			return apology("please enter ticker/symbol", 400)

		# Retrieves data based on symbol
		quote = lookup(request.form.get("symbol"))

		# Ensures that the symbol contains a match
		if not quote:
			return apology("symbol/ticker not found", 400)

		# Converts price into dollar format

		# Calls to display details of the symbol
		return render_template("quoted.html", quoted=quote)

	return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
	"""Register user"""

	if request.method == "POST":

		# Ensure username was submitted
		if not request.form.get("username"):
			return apology("must provide username", 400)

		# Ensure password was submitted
		if not request.form.get("password"):
			return apology("must provide password", 400)
		elif request.form.get("password") != request.form.get("confirmation"):
			return apology("passwords do not match", 400)

		# Query database for username
		row = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

		if len(row) != 0:
			return apology("username already exists", 400)

		# Query to insert login details to database
		db.execute("INSERT INTO users(username, hash) VALUES (?, ?)", request.form.get("username"), generate_password_hash(request.form.get("password")))

		# automatically sets user as logged
		session["user_id"] = db.execute("SELECT id FROM users WHERE username = ?", request.form.get("username"))[0].get("id")

		# Redirect user to home page
		flash("Registered!")
		return redirect("/")

	return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
	"""Sell shares of stock"""
	if request.method == "POST":

		# Ensure symbol was submitted
		if not request.form.get("symbol"):
			return apology("please enter symbol", 400)

		if not request.form.get("shares"):
			return apology("please enter number of shares", 400)

		share = int(request.form.get("shares"))

		# Retrieves data based on symbol
		quote = lookup(request.form.get("symbol"))

		if not quote:
			return apology("invalid symbol", 400)

		cost = float(quote['price'] * share)

		try:
			rows = db.execute("SELECT SUM(shares) FROM stocks WHERE symbol = ?", quote['symbol'])
			if len(rows) < 1:
				return apology("no shares found", 400)
		except:
			return apology("no shares owned", 400)

		rows = rows[0].get("SUM(shares)")
		if rows < share:
			return apology("not enough shares to sell", 400)

		now = datetime.datetime.now()
		db.execute("INSERT INTO stocks (user, symbol, shares, price, date, time) VALUES (?, ?, ?, ?, ?, ?)", session["user_id"], quote['symbol'], 0 - share, quote['price'], now.strftime("%Y-%m-%d"), now.strftime("%H:%M:%S"))


		# Query user's balance from database
		balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
		balance = balance[0]["cash"]

		# Updates user's balance
		db.execute("UPDATE users SET cash = ? WHERE id = ?", balance + cost, session["user_id"])

		flash("Sold!")
		return redirect("/")


	try:
		rows = db.execute("SELECT symbol FROM stocks WHERE user = ?", session["user_id"])
	except:
		rows = []

	return render_template("sell.html", symbols=rows)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def password():
	if request.method == "POST":
		old_pass = request.form.get("old_pass")
		pass2 = request.form.get("password")

		if not old_pass or not pass2:
			return apology("please enter old and new password", 403)

		# Query database for username
		rows = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])


		# Ensure username exists and password is correct
		if len(rows) != 1 or not check_password_hash(rows[0]["hash"], old_pass):
			return apology("invalid old password", 403)

		if pass2 != request.form.get("confirmation"):
			return apology("new passwords don't match", 403)

		db.execute("UPDATE users SET hash = ? WHERE id = ?", generate_password_hash(pass2), session["user_id"])

		flash("Password changed!")
		return redirect("/")

	return render_template("changepass.html")



@app.route("/reset")
def reset():
	try:
		db.execute("DROP TABLE stocks")
	except:
		print("No table found")

	db.execute("DELETE FROM users")

	return redirect("/logout")


def errorhandler(e):
	"""Handle error"""
	if not isinstance(e, HTTPException):
		e = InternalServerError()
	return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
	app.errorhandler(code)(errorhandler)