
n = 0

while n < 1 or n > 8:
     s = input("Height: ")
     if s.isdigit():
          n = int(s)


for i in range(1,n + 1):
     s = str(" " * (n - i) + "#" * i + "  " + "#" * i)
     print(s)