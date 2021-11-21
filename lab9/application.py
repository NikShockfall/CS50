import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")

@app.route("/", methods=["GET", "POST"])
def index():
    birthdays = db.execute("SELECT * FROM birthdays")
    message = ""

    if request.method == "POST":
        # TODO: Add the user's entry into the database
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")
        if name and month.isnumeric() and day.isnumeric():
            if 0 < int(month) < 13 and 0 < int(day) < 32:
                db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)
                return redirect("/")
            #else:
                #message = "Month/Day outside range."
        #else:
            #message = "Please fill in all the fields."
        return render_template("index.html", birthdays=birthdays, exception=message)

    else:
        # TODO: Display the entries in the database on index.html
        return render_template("index.html", birthdays=birthdays, exception=message)

@app.route("/remove", methods=["POST"])
def remove():
    name = request.form.get("remove")
    db.execute("DELETE FROM birthdays WHERE name = ?", name)
    return redirect("/")
