import argparse
import os
import csv

def search_each_row(output, row, key, value):
    if key == 'year':
        if row[0] == value:
            output.append(row)
    elif key == 'cause':
        if row[1].find(value) != -1 or row[1].find(value.lower()) != -1 or row[1].find(value.title()) != -1 or row[1].find(value.upper()) != -1 or row[2].find(value) != -1 or row[2].find(value.lower()) != -1 or row[2].find(value.title()) != -1 or row[2].find(value.upper()) != -1:
            output.append(row)
    elif key == 'state':
        if row[3].find(value) != -1 or row[3].find(value.lower()) != -1 or row[3].find(value.title()) != -1 or row[3].find(value.upper()) != -1:
            output.append(row)
    elif key == 'deaths':
        if int(row[4]) >= int(value):
            output.append(row)
    elif key == 'age_adjusted':
        if int(row[5]) >= int(value):
            output.append(row)

    return output


def find(file_search, searchable, key, value):
    output = []

    if file_search:
        with open(searchable, 'r') as f:
            f.readline()
            csvreader = csv.reader(f, delimiter=',')

            for row in csvreader:
                output = search_each_row(output, row, key, value)

            return output
    else:
        for i in range(len(searchable)):
            output = search_each_row(output, searchable[i], key, value)

        return output    



def print_formatted(result_list):
    length = len(result_list)

    if length > 0:
        for i in range(length):
            output = []
            
            for word in result_list[i]:
                output.append('{:>25}'.format(word))

            print(''.join(output))
    else:
        print('No results to display for the given query.')

#=====================================================================================



# set up the command line interface 
parser = argparse.ArgumentParser(description='CSV File Search') # initialize parser
parser.add_argument('file', help='path to the CSV file')

# introduce a mutually inclusive group of commands that can be used individually only
parser.add_argument('-y', '--year', help='search by year')
parser.add_argument('-c', '--cause', help='search by cause')
parser.add_argument('-s', '--state', help='search by state')
parser.add_argument('-d', '--deaths', help='search by number of deaths (greater than or equal to)')
parser.add_argument('-ad', '--age_adjusted', help='search by age adjusted death-rate (greater than or equal to)')

# get the arguments given
args = parser.parse_args()
# get a dictionary of the passed arguments
dict_args = vars(args)

result = None

# iterate through the dictionary
for key, value in dict_args.items():
    if key == 'file':
        continue
    elif value is None:
        continue
    else:
        if result is None:
            result = find(True, dict_args['file'], key, value)
        else:
            result = find(False, result, key, value)

print_formatted(result)