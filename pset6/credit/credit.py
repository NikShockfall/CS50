from cs50 import get_int

MIN = 4000000000000
MAX = 5600000000000000


def main():
    card = get_int("Number: ")

    if(validate(card)):
        prefix = int(str(card)[0] + str(card)[1])

        if prefix in [34, 37]:
            print("AMEX")
        elif prefix in range(40,50):
            print("VISA")
        elif prefix in range(51,56):
            print("MASTERCARD")
        else:
            print("INVALID")


def validate(card):
    if card < MIN or card > MAX:
        # print("out of bounds")
        return False

    card = str(card)
    digits = []

    for i in range(len(card), 16):
        digits.append(0)

    for d in card:
        digits.append(int(d))

    formula = 0
    for i in range(0, len(digits)):
        formula += digits[i]

        if i % 2 == 0:
            formula += digits[i]

            if digits[i] > 4:
                formula -= 9

    # print(formula)
    if formula % 10 == 0:
        return True
    else:
        return False


if __name__ == "__main__":
    main()