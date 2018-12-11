# import necessary modules
import argparse
import csv

def search_each_row(output, row, key, value):
	"""
		This is a helper function that takes in the current row of the csv file, the current output list and the key and value pair.
		Then, it matches the key and value pairs against the appropriate rows of the csv file. If a match is found, the output list is updated
		and sent back to the caller.

		String checks are done in both titlecase (e.g. Title), uppercase (e.g. TITLE) and lowercase (e.g. title)
	"""
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
		if float(row[4]) >= float(value):
			output.append(row)
	elif key == 'age_adjusted':
		if float(row[5]) >= float(value):
			output.append(row)

	return output


def find(file_search, searchable, key, value):
	"""
		Helper function that takes in a file or an iterable, depending on the number of search parameters specified. Depends on search_each_row().
	"""
	output = []

	if file_search:
		# if the searchable variable is a file path
		with open(searchable, 'r') as f:
			f.readline()
			csvreader = csv.reader(f, delimiter=',')

			for row in csvreader:
				output = search_each_row(output, row, key, value)

			return output
	else:
		# if the searchable variable is a list that already has some search results and we want to further search on that data
		for i in range(len(searchable)):
			output = search_each_row(output, searchable[i], key, value)

		return output    



def print_formatted(result_list):
	"""
		Helper function that prints the query search results with appropriate padding and in a prettier manner.
	"""
	if result_list is None:
		return print('No search parameters were passed!')
	
	length = len(result_list)

	if length > 0:
		for i in range(length):
			output = []
			
			for word in result_list[i]:
				output.append('{:<25}'.format(word))

			print('\t'.join(output))
	else:
		print('No results to display for the given query.')

#=====================================================================================



# set up the command line interface 

# initialize parser
parser = argparse.ArgumentParser(description='CSV File Search') 

# mandatory argument: path to the csv file
parser.add_argument('file', help='path to the CSV file')

# introduce a mutually inclusive group of commands that can be used individually or in a combination
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
	# skip the file value, that is not a search parameter
	if key == 'file':
		continue
	elif value is None:
	# skip any key value pairs that are none, user hasn't wanted to search using those values
		continue
	else:
		if result is None:
			# if this is the first search result, store the results in the variable
			result = find(True, dict_args['file'], key, value)
		else:
			# this fires if there were more than one search paramters (e.g. year AND deaths, year, deaths AND state etc)
			result = find(False, result, key, value)

# print the results
print_formatted(result)