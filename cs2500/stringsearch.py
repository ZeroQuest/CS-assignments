#!/usr/bin/python3


def string_search(text, pattern):
    n = len(text)
    m = len(pattern)
    comparisons = 0

    for i in range (n-m+1):
        j=0
        comparisons+=1
        while j < m and pattern[j] == text[i+j]:
            j+=1
            comparisons+=1

        if j == m:
            return i, comparisons

    return -1, comparisons

def main():
    default_text = "THE_MIND_IS_EVERYTHING.WHAT_YOU_THINK-YOU_BECOME"
    default_pattern = "THING"

    text = input("Please enter the text: ")
    pattern = input("Please enter the pattern: ")

    if text == "":
        text = default_text

    if pattern == "":
        pattern = default_pattern


    result, comparisons = string_search(text, pattern)

    if result != -1:
        print(f"The pattern '{pattern}' is found at index {result}.")
        print(f"Number of comparisons: {comparisons}")
    else:
        print(f"The pattern '{pattern}' is not found in the text.")
        print(f"Number of comparisons: {comparisons}")

if __name__ == "__main__":
    main()
