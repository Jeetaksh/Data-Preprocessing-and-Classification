#include <bits/stdc++.h>
using namespace std;

/*
    Authors: Mukul Malik, Jeetaksh Gandhi
    Purpose: We have the original dataset named "dataset.csv", consisting of 13 features (14 columns) and 48,882 data rows.
             Many columns however have categorical data. This program maps them into an integral value.

             Suppose a column has 3 possible categorical values --> 'yes', 'no', and 'maybe'. We wish to map them to 0, 1, 2
             respectively. Then you have to pass the input as : yes, no, maybe,
             It will automatically label them as 0, 1, 2 respectively in order of input.

             Now, say there is another column with values --> 'low', 'medium', 'high'. You also want to map it as 0, 1, 2.
             Then in the input string, put a '#' when values of a particular column are finished and a next column values are 
             starting.

             So now the input string is : 'yes', 'no', 'maybe', '#', 'low', 'medium',' high',
             It thus labels them as        0      1     2     'RESET'  0      1         2

             Each of our categorical columns have unique value domains thus this works perfectly. There is no redundancy.

             Our categorical columns to be mapped into integers are: WORKCLASS, MARITAL_STATUS, Occupation, Relationship, Race, Gender, Native_Country

             Thus, input map string for our dataset IS: (avoid the newline characters! They are only here for better formatting. No newline character for input of program.)
             Private, Self-emp-not-inc, Self-emp-inc, Federal-gov, Local-gov, State-gov, Without-pay, Never-worked, #,  
             Married-civ-spouse, Divorced, Never-married, Separated, Widowed, Married-spouse-absent, Married-AF-spouse, #, 
             Tech-support, Craft-repair, Other-service, Sales, Exec-managerial, Prof-specialty, Handlers-cleaners, Machine-op-inspct, Adm-clerical, Farming-fishing, Transport-moving, Priv-house-serv, Protective-serv, Armed-Forces, #, 
             Wife, Own-child, Husband, Not-in-family, Other-relative, Unmarried, #, 
             White, Asian-Pac-Islander, Amer-Indian-Eskimo, Other, Black, #, 
             Female, Male, #, 
             United-States, Cambodia, England, Puerto-Rico, Canada, Germany, Outlying-US(Guam-USVI-etc), 
             India, Japan, Greece, South, China, Cuba, Iran, Honduras, Philippines, Italy, Poland, Jamaica, Vietnam, Mexico, Portugal, 
             Ireland, France, Dominican-Republic, Laos, Ecuador, Taiwan, Haiti, Columbia, Hungary, Guatemala, Nicaragua, Scotland, Thailand, 
             Yugoslavia, El-Salvador, Trinadad&Tobago, Peru, Hong, Holand-Netherlands, #, 
             <=50K, >50K,

             Now, it stores the processed dataset in a new file called 'final.csv'.
             This program has been extensively debugged and tested.

    Compile Instructions: You require the G++ Compiler. Compile command:
             g++ ./mapper.cpp -o mapper.exe

             Then put dataset.csv into this folder and run:
             ./mapper.exe

             And enter the string above as input after that and voila! It only takes about a second.
             Made entirely from scratch! We believe in making our own tools.
*/

int main()
{
    // First input the processing string
    string input;
    cout << "Please enter mapping string:    ";
    getline(cin, input);

    unordered_map<string, int> map;
    //            <categorical value, integer mapping> pairs

    int count = 0;                      // to maintain indexing
    string word = "";

    for(int i = 0; i < input.size(); i++)
    {
        if(input[i] == ',')
        {
            // Current word has ended. Brace for a new word!
            map[word] = count++;
            word = "";                  // reset word variable for new word parsing.
        }

        else if(input[i] == '#')
        {
            // new column values starting so reset count to 0 to reset indexing to 0.
            count = 0;
            i++;
        }

        else if(input[i] != ' ')        // ignore spaces in mapping string.
        {
            word += input[i];
        }
    }

    // Now everything is in map. For each categorical value, we have its numerical mapping in the map.

    FILE *fp = fopen("dataset.csv", "r+");
    if(fp == NULL)
    {
        cout << "Exiting!";
        return 0;
    }

    char** data = new char*[50000];     // First load entire content of the dataset into a 2D character array where each row of the array represents each row of the dataset for easy manipulation.
                                        // We require 50,000 rows as we have 48,882 rows inside our dataset.

    for (int k = 0; k < 50000; ++k) 
    {
        data[k] = new char[500];        // Each row should store 500 characters at max. This will suffice for all rows.
    }

    int i = 0;
    cout << "\n             --- Reading Dataset--- \n" << endl;

    while(fscanf(fp, "%500[^\n]\n", data[i++]) != EOF)
    {
        // Read each row and store into rows of our 2d char array from data[0], data[1]... till data[48881]! fscanf terminates at newline. Thus it will execute 48882 times.
    }
    
    fclose(fp);

    cout << "\n             --- Beginning Data Manipulation.\n" << endl;

    FILE *fpp = fopen("final.csv", "w+");   // Create a new csv file and output updated rows into it. A csv is basically a comma separated value file.

    for(int j = 0; j < i; j++)              // Process row-by-row
    {
        int m = 0;
        string aux = "";
        int comma = 0;

        if(j == 0)                          // Output first row as it is SINCE it is just column names!
        {
            fprintf(fpp, "%s\n", data[j]);
            continue;
        }

        while(data[j][m] != '\0')           // Execute/Parse till we encounter end of the row.
        {
            if(data[j][m] == ' ' || data[j][m] == '?' )     // Output spaces and '?' (Missing) values as is.
            {
                fprintf(fpp, "%c", data[j][m]);
                m++;
                continue;
            }

            if(data[j][m] >= 48 && data[j][m] <= 57)        // Output numerical values as is since they don't have to mapped.
            {
                fprintf(fpp, "%c", data[j][m]);
                m++;
                continue;
            }

            if(data[j][m] == ',')                           // If you encounter a comma, then see if previous field read was number or text. If number (string length read is 0 as we are
                                                            // not including numbers in the string), then nothing to do. Put comma as is. If a textual, output its integral mapping before comma!
            {
                if(aux.length() > 0)
                {
                    fprintf(fpp, "%d", map[aux]);
                    aux = "";
                }
                fprintf(fpp, "%c", data[j][m]);             // outputs ','
                m++;
                comma++;
                continue;
            }

            if(comma == 13)                                 // If this is the last comma, then we know last column is textual. Read the rest and map and output it. And break the while loop.
            {
                for(int n = m; data[j][n] != '\0'; n++)
                {
                    if(data[j][n] == '.')
                    {
                        continue;
                    }
                    aux += data[j][n];
                }
                fprintf(fpp, " %d\n", map[aux]);
                aux = "";
                break;
            }

            else                                            // Parsing a character, add it to our string to later recognize the full categorical value to map to integer.
            {
                aux += data[j][m];
                m++;
            }
        }
    }

    fclose(fpp);                                            // And we're done!

    cout << "\n Finished Processing \n" << endl;

    // Clear memory
    for (int k = 0; k < 50000; ++k) 
    {
        delete[] data[k];
    }
    delete[] data;

    return 0;
}
