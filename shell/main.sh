#!/bin/sh

# set up the arguments
for i in "$@"
do
case $i in
    -y=*|--year=*)      # year (column 1)
    YEAR="${i#*=}"
    ;;
    -c=*|--cause=*)     # cause (column 2 or column 3)
    CAUSE="${i#*=}"
    ;;
    -s=*|--state=*)     # name of the state (column 4)
    STATE="${i#*=}"
    ;;
    -d=*|--deaths=*)    # number of deaths (greater than or equal to) (column 5)
    DEATHS="${i#*=}"
    ;;
    -ad=*|--age_adjusted=*)     # age adjusted death rate (greater than or equal to) (column 6)
    AGE_ADJUSTED="${i#*=}"  
    ;;
    *)
          # unknown option
    ;;
esac
done

# this line preserves the newline characters while appending the results to the variable RESULT
IFS=

# Using awk for numerical data comparison as grep does not support that
# This block will contain all the necessary definitions

# -F "\"*,\"*" specify the delimiter between two columns (here it is the comma (,))

# -vFS=, -vOFS=, \
# -vYEAR=$YEAR \
# -vCAUSE=$CAUSE \
# -vSTATE=$STATE \
# -vDEATHS=$DEATHS \
# -vAGE_ADJUSTED=$AGE_ADJUSTED \
# these are options that set up the internal variable of awk with the variables used to store the arguments passed by the user.
# In this way, we do not need to use $ before variable names inside awk

# there can be three cases:
# User passed both "number of deaths" and "age-adjusted death rate"
# User passed only one of either "number of deaths" or "age-adjusted death rate"
# User did not pass any of the two arguments
# We need to check for the first two cases while comparing and segregating the columns from the csv file, which is done in the if blocks

# We use negative logic here. Instead of saying "if (csv column number of deaths >= user argument number of deaths) { print this line }",
# we say "if (csv column number of deaths < user argument number of deaths) { skip this line }"

# FPAT='([^,]+)|("[^"]+")'
# The csv file has multiple rows where the second column has commas in it
# e.g. "Accidents (unintentional injuries) (V01-X59,Y85-Y86)"
# this confuses the parser and it splits those lines on those commas as well, even though the values are surrounded by "" quotation marks.
# To avoid this, we use the regex ([^,]+)|("[^"]+") which tells the parser to ignore commas inside "" quotation marks

# print
# if the next; command did not get fired inside the if blocks, it means that this row of the csv file matches user criteria
# therefore, we store it in the variable RESULT

# $1 on line 98 is the path to the CSV file, this is a bash variable

RESULT=$(awk -F "\"*,\"*" \
    -vFS=, -vOFS=, \
    -vYEAR=$YEAR \
    -vCAUSE=$CAUSE \
    -vSTATE=$STATE \
    -vDEATHS=$DEATHS \
    -vAGE_ADJUSTED=$AGE_ADJUSTED \
'{
    if (length(YEAR) != 0) {
        if ($1 != YEAR) {
            next;
        }
    }

    if (length(DEATHS) != 0 && length(AGE_ADJUSTED) != 0) {
        if ($5 < DEATHS || $6 < AGE_ADJUSTED) {
            next;
        }
    }

    if (length(DEATHS) != 0) {
        if ($5 < DEATHS) {
            next;
        }
    }

    if (length(AGE_ADJUSTED) != 0) {
        if ($6 < AGE_ADJUSTED) {
            next;
        }
    }
    print
}' FPAT='([^,]+)|("[^"]+")' $1)


# we got our numerically specific data (if the user passed -d or -ad arguments). 
# Now we need to further sort out the data with any additional string arguments that the user may have passed.
# Cause of Death
# Name of the State etc.
# This can be accomplished by grep on the RESULT variable that contains the initial search data

echo $RESULT | grep "$CAUSE" | grep "$STATE"