'''
Author: FunctionSir
License: AGPLv3
Date: 2024-12-17 08:36:45
LastEditTime: 2024-12-19 14:39:07
LastEditors: FunctionSir
Description: To make it.
FilePath: /crse-proj-ds/hash-table/make.py
'''

import os
import time

out_file = "hash-table"
conflit_handlers_list = ["CHAINING", "LINEAR", "SQUARE"]
hash_functions_list = ["BASE26", "SUM", "ASCII"]

print("Config and build \"hash-table\"...")

print("Hash functions: "+" ".join(hash_functions_list))
hash_function = input("Select the hash function: ")
if hash_function not in hash_functions_list:
    print("ERROR: No such hash function!")
    exit(1)

print("Conflict handlers: "+" ".join(conflit_handlers_list))
conflit_handler = input("Select the conflict handling method: ")
if conflit_handler not in conflit_handlers_list:
    print("ERROR: No such conflit handler!")
    exit(1)

print("Start to build...")
begin = time.time_ns()
os.system("gcc main.c"+" -D "+conflit_handler +
          " -D "+hash_function+" -O3 -o "+out_file)
end = time.time_ns()
print("Done. Used "+str((end-begin)/1000/1000)+"ms.")
