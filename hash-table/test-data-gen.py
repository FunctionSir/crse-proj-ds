'''
Author: FunctionSir
License: AGPLv3
Date: 2024-12-17 08:52:03
LastEditTime: 2024-12-19 08:18:24
LastEditors: FunctionSir
Description: To gen data for testing.
FilePath: /crse-proj-ds/hash-table/test-data-gen.py
'''

import random

x = int(input())

print(x)
for i in range(x):
    tl = random.sample(
        'QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm', 30)
    print("".join(tl))
