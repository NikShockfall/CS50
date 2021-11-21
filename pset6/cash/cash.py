import cs50

while True:
     change = cs50.get_float("Change owed: ")
     if change > 0:
          break

cents = int(change * 100)
coins = 0

for i in [25, 10, 5, 1]:
     while cents >= i:
          cents -= i
          coins += 1

print(coins)
