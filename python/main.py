import argparse
import os
import csv

def find(file_path, argument, index):
    with open(file_path, 'r') as f:
        f.readline()
        csvreader = csv.reader(f, delimiter=',')
        
        count = 0

        if index == 4 or index == 5:
            for row in csvreader:
                if int(row[index]) >= int(argument):
                    count += 1
                    print(row)
        else:
            for row in csvreader:
                if row[index].find(argument) != -1 or row[index].find(argument.lower()) != -1 or row[index].find(argument.upper()) != -1:
                    count += 1
                    print(row)

        if count == 0:
            print('{} not found in the provided csv file under the given criteria.'.format(argument))


# set up the command line interface 
parser = argparse.ArgumentParser(description='CSV File Search') # initialize parser
parser.add_argument('file', help='path to the CSV file')

# introduce a mutually exclusive group of commands that can be used individually only
group = parser.add_mutually_exclusive_group()
group.add_argument('-y', '--year', help='search by year')
group.add_argument('-c', '--cause', help='search by cause')
group.add_argument('-s', '--state', help='search by state')
group.add_argument('-d', '--deaths', help='search by number of deaths (greater than or equal to)')
group.add_argument('-ad', '--age_adjusted', help='search by age adjusted death-rate (greater than or equal to)')

# get the arguments given
args = parser.parse_args()
dict_args = vars(args)
print(args)

for key, value in dict_args.items():
    if key == 'file':
        continue
    if value is not None:
        print('Searching for {} under {}'.format(value, key))

if args.year:    
    find(args.file, args.year, 0)
elif args.cause:    
    find(args.file, args.cause, 2)
elif args.state:    
    find(args.file, args.state, 3)
elif args.deaths:
    find(args.file, args.deaths, 4)
elif args.age_adjusted:
    find(args.file, args.age_adjusted, 5)
    


