import random

def generate_random_list(length, total):
    result = [1] * length  
    total -= length  
    for _ in range(total):
        result[random.randint(0, length - 1)] += 1
    random.shuffle(result)
    return result

m, n = map(int, input().split())
total = int(input())

print(m, n, sep = ",")

list_m = generate_random_list(m, total)
print(",".join(map(str, list_m)))

list_n = generate_random_list(n, total)
print(",".join(map(str, list_n)))

for _ in range(m):
    row = generate_random_list(n, random.randint(n, n * 10))
    print(",".join(map(str, row)))
