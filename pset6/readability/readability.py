from cs50 import get_string



def main():
    text = get_string("Text: ")
    grade = index(text)
    if grade < 1:
        print("Before Grade 1")
    elif 0 < grade < 17:
        print(f"Grade {grade}")
    else:
        print("Grade 16+")


def index(text):
    words = len(text.rsplit(" "))
    letters = 0
    sentences = 0

    for c in text:
        if c.isalpha():
            letters += 1
        elif c in ["?", "!", "."]:
            sentences += 1

    return round(0.0588 * letters * 100 / words - 0.296 * sentences * 100 / words - 15.8)


if __name__ == "__main__":
    main()