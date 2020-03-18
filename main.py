import sys
from pprint import pprint

if len(sys.argv) != 2:
    print("Usage: solver <test>")

if len(sys.argv[1]) != 1:
    print("Unknown testcase!")

case = sys.argv[1][0]
if case == '0':
    f = open("tests/0_example.txt", "r")
elif case == '1':
    f = open("tests/a_solar.txt", "r")
elif case == '2':
    f = open("tests/b_dream.txt", "r")
elif case == '3':
    f = open("tests/c_soup.txt", "r")
elif case == '4':
    f = open("tests/d_maelstrom.txt", "r")
elif case == '5':
    f = open("tests/e_igloos.txt", "r")
elif case == '6':
    f = open("tests/f_glitch.txt", "r")

# get office dimentions
office_dimentions = f.readline()
width = int(office_dimentions[0])
height = int(office_dimentions[2])

# get tile types and build map matrix
office = []
for i in range(height):
    line = f.readline()
    row = []
    for i in range(width):
        row.append(line[i])
    office.append(row)

# read programmers
tot_programmers = f.readline()
for i in range(tot_programmers):
    info = f.readline()
    info = info.split(" ")

# read managers
