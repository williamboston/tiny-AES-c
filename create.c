from random import choice

target_size = 1024 ** 3  # 1 GiB, see https://en.wikipedia.org/wiki/Gibibyte
# convert to string just the once
numbers = [str(i) for i in range(100)]

with open('number.txt', 'a') as f_out:
    f_out.write(choice(numbers))
    while f_out.tell() < target_size:
        f_out.write(',{}'.format(choice(numbers)))