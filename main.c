#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FunctionNode
{
    char name[100];
    int cyclomaticComplexity;
    struct FunctionNode *next;
} FunctionNode;

typedef struct FileNode
{
    char name[100];
    FunctionNode *functions;
    struct FileNode *next;
} FileNode;

typedef struct CodeMetrics
{
    int loc;
    int numFunctions;
    int cyclomaticComplexity;
} CodeMetrics;

FunctionNode *createFunctionNode(const char *name)
{
    FunctionNode *newNode = (FunctionNode *)malloc(sizeof(FunctionNode));
    if (newNode != NULL)
    {
        strcpy(newNode->name, name);
        newNode->cyclomaticComplexity = 0;
        newNode->next = NULL;
    }
    return newNode;
}

FileNode *createFileNode(const char *name)
{
    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    if (newNode != NULL)
    {
        strcpy(newNode->name, name);
        newNode->functions = NULL;
        newNode->next = NULL;
    }
    return newNode;
}

void parseSourceCode(const char *filename, FileNode **root)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        if (strstr(buffer, "void"))
        {
            char functionName[100];
            sscanf(buffer, "%*s %s", functionName);

            FunctionNode *newFunction = createFunctionNode(functionName);

            FileNode *currentFile = *root;
            if (currentFile == NULL)
            {
                FileNode *newFile = createFileNode(filename);
                newFile->functions = newFunction;
                *root = newFile;
            }
            else
            {
                while (currentFile->next != NULL && strcmp(currentFile->name, filename) != 0)
                {
                    currentFile = currentFile->next;
                }
                if (strcmp(currentFile->name, filename) != 0)
                {
                    FileNode *newFile = createFileNode(filename);
                    newFile->functions = newFunction;
                    currentFile->next = newFile;
                }
                else
                {
                    if (currentFile->functions == NULL)
                    {
                        currentFile->functions = newFunction;
                    }
                    else
                    {
                        FunctionNode *currentFunction = currentFile->functions;
                        while (currentFunction->next != NULL)
                        {
                            currentFunction = currentFunction->next;
                        }
                        currentFunction->next = newFunction;
                    }
                }
            }
        }
    }

    fclose(file);
}

CodeMetrics calculateMetrics(FileNode *root)
{
    CodeMetrics metrics;
    metrics.loc = 0;
    metrics.numFunctions = 0;
    metrics.cyclomaticComplexity = 0;

    FileNode *currentFile = root;
    while (currentFile != NULL)
    {
        FILE *file = fopen(currentFile->name, "r");
        if (file != NULL)
        {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), file) != NULL)
            {
                metrics.loc++;
            }
            fclose(file);
        }

        FunctionNode *currentFunction = currentFile->functions;
        while (currentFunction != NULL)
        {
            metrics.numFunctions++;
            metrics.cyclomaticComplexity += currentFunction->cyclomaticComplexity;
            currentFunction = currentFunction->next;
        }

        currentFile = currentFile->next;
    }

    return metrics;
}

int countOccurrences(const char *str, char c)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c)
        {
            count++;
        }
    }
    return count;
}

int calculateFunctionCyclomaticComplexity(const char *filename, const char *functionName)
{
    // Open the source code file
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // Handle file opening error
        return -1;
    }

    // Initialize variables
    int complexity = 1; // Initialize complexity to 1 for the function entry point
    char buffer[1024];
    int insideFunction = 0;
    int bracketCount = 0;
    int insideComment = 0;
    int functionNestingLevel = 0;

    // Loop through each line of the source code
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        // Check for comments
        if (strstr(buffer, "/*"))
            insideComment = 1;

        if (!insideComment)
        {
            // Check if we're inside the function
            if (insideFunction)
            {
                // Count decision points such as control flow statements and logical operators
                if (strstr(buffer, "if") || strstr(buffer, "else") ||
                    strstr(buffer, "while") || strstr(buffer, "for") ||
                    strstr(buffer, "case") || strstr(buffer, "default") ||
                    strstr(buffer, "#if") || strstr(buffer, "#elif") || strstr(buffer, "#else"))
                {
                    complexity++;
                }
            }

            // Count opening and closing brackets to determine function nesting level
            bracketCount += countOccurrences(buffer, '{');
            bracketCount -= countOccurrences(buffer, '}');

            // Check if the current line defines the function
            char functionDef[100];
            if (sscanf(buffer, "%*s %s", functionDef) == 1 && strcmp(functionDef, functionName) == 0)
            {
                insideFunction = 1; // Set insideFunction flag
                functionNestingLevel++; // Increment nesting level
            }

            // Check if we've reached the end of the function
            if (insideFunction && bracketCount == 0 && functionNestingLevel == 1)
            {
                insideFunction = 0; // Reset insideFunction flag
            }

            // Adjust function nesting level if encountering closing brackets
            if (insideFunction && bracketCount < 0)
            {
                functionNestingLevel--; // Decrement nesting level
                if (functionNestingLevel == 0)
                {
                    insideFunction = 0; // Reset insideFunction flag if exiting the function
                }
            }
        }

        // Check for end of comment block
        if (strstr(buffer, "*/"))
            insideComment = 0;
    }

    // Close the file
    fclose(file);

    // Return the calculated cyclomatic complexity
    return complexity;
}


void calculateFunctionComplexities(FileNode *root)
{
    FileNode *currentFile = root;
    while (currentFile != NULL)
    {
        FunctionNode *currentFunction = currentFile->functions;
        while (currentFunction != NULL)
        {
            currentFunction->cyclomaticComplexity = calculateFunctionCyclomaticComplexity(currentFile->name, currentFunction->name);
            currentFunction = currentFunction->next;
        }
        currentFile = currentFile->next;
    }
}

void freeFileNodes(FileNode *root)
{
    while (root != NULL)
    {
        FileNode *tempFile = root;
        root = root->next;
        FunctionNode *currentFunction = tempFile->functions;
        while (currentFunction != NULL)
        {
            FunctionNode *tempFunction = currentFunction;
            currentFunction = currentFunction->next;
            free(tempFunction);
        }
        free(tempFile);
    }
}

int main()
{
    FileNode *root = NULL;
    const char *filename = "example2.c";
    parseSourceCode(filename, &root);

    calculateFunctionComplexities(root);
    CodeMetrics metrics = calculateMetrics(root);

    char outputFilename[100];
    strcpy(outputFilename, filename);
    strcat(outputFilename, "_metrics.txt");
    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL)
    {
        printf("Error: Unable to create file %s\n", outputFilename);
        return 1;
    }

    fprintf(outputFile, "Metrics for %s:\n", filename);
    fprintf(outputFile, "Lines of code: %d\n", metrics.loc);
    fprintf(outputFile, "Number of functions: %d\n", metrics.numFunctions);
    fprintf(outputFile, "Cyclomatic complexity: %d\n", metrics.cyclomaticComplexity);

    fclose(outputFile);
    freeFileNodes(root);
    return 0;
}