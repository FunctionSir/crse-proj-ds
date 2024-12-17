'''
Author: FunctionSir
License: AGPLv3
Date: 2024-12-17 08:52:03
LastEditTime: 2024-12-17 08:54:52
LastEditors: FunctionSir
Description: To gen data for test.
FilePath: /crse-proj-ds/hash-table/test-data-gen.py
'''

import random

x = int(input("Input test cases count: "))

print(x)
for i in range(x):
    tl = random.sample(
        'QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm', 30)
    print("".join(tl))
