#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int isArrayEmpty(int *arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (arr[i] != -1)
        {
            return 0;
        }
    }
    return 1;
}

void printMatrix(int **arr, int n, int m)
{
    int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void maxHeapify(int *arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i)
    {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        maxHeapify(arr, n, largest);
    }
}

void buildMaxHeap(int *arr, int n)
{
    int i;
    for (i = n / 2 - 1; i >= 0; i--)
        maxHeapify(arr, n, i);
}

int main()
{
    int numRows;
    int i, j;

    int *extinctionOrder = NULL;
    int extinctionIndex = 0;

    printf("Enter the number of rows: ");
    scanf("%d", &numRows);

    extinctionOrder = (int *)malloc(numRows * sizeof(int));
    if (extinctionOrder == NULL) {
        printf("Memory allocation failed for extinctionOrder.\n");
        return 1;
    }


    int maxCols = 0;
    int *rowSizes = (int *)malloc(numRows * sizeof(int));
     if (rowSizes == NULL) {
        printf("Memory allocation failed for rowSizes.\n");
        free(extinctionOrder);
        return 1;
    }
    int totalSize = 0;
    for (i = 0; i < numRows; i++)
    {
        printf("Enter the number of columns in row %d: ", i);
        scanf("%d", &rowSizes[i]);
        totalSize += rowSizes[i];
        if (rowSizes[i] > maxCols)
        {
            maxCols = rowSizes[i];
        }
    }

    int **matrix = (int **)malloc(numRows * sizeof(int *));
     if (matrix == NULL) {
        printf("Memory allocation failed for matrix rows.\n");
        free(extinctionOrder);
        free(rowSizes);
        return 1;
    }
    for (i = 0; i < numRows; i++)
    {
        matrix[i] = (int *)malloc(maxCols * sizeof(int));
        if (matrix[i] == NULL) {
             printf("Memory allocation failed for matrix column %d.\n", i);
             for(j = 0; j < i; j++) free(matrix[j]);
             free(matrix);
             free(extinctionOrder);
             free(rowSizes);
             return 1;
        }
    }

    for (i = 0; i < numRows; i++)
    {
        for (j = 0; j < maxCols; j++)
        {
            matrix[i][j] = -1;
        }
    }

    int *randomNumbers = (int *)malloc(numRows * maxCols * sizeof(int));
     if (randomNumbers == NULL) {
        printf("Memory allocation failed for randomNumbers.\n");
        for(i = 0; i < numRows; i++) free(matrix[i]);
        free(matrix);
        free(extinctionOrder);
        free(rowSizes);
        return 1;
    }
    for (i = 0; i < numRows * maxCols; i++)
    {
        randomNumbers[i] = i;
    }

    srand(time(NULL));
    for (i = 0; i < numRows * maxCols; i++)
    {
        int randomIndex = rand() % (numRows * maxCols);
        int temp = randomNumbers[i];
        randomNumbers[i] = randomNumbers[randomIndex];
        randomNumbers[randomIndex] = temp;
    }

    int randomNumIndex = 0;
    for (i = 0; i < numRows; i++)
    {
        for (j = 0; j < rowSizes[i]; j++)
        {
            matrix[i][j] = randomNumbers[randomNumIndex];
            randomNumIndex++;
        }
    }

    printf("Initial matrix:\n\n");
    printMatrix(matrix, numRows, maxCols);

    for (i = 0; i < numRows; i++)
    {
        buildMaxHeap(matrix[i], maxCols);
    }

    printf("\n\nMatrix after converting each row to max heap:\n\n");
    printMatrix(matrix, numRows, maxCols);

    while (totalSize > 0)
    {
        int currentMax = -1;
        int maxRowIndex = -1;

        printf("Checked values (row roots): ");
        for (i = 0; i < numRows; i++)
        {
            if(matrix[i][0] != -1){
                 printf("%d ", matrix[i][0]);
                 if (matrix[i][0] > currentMax)
                 {
                     currentMax = matrix[i][0];
                     maxRowIndex = i;
                 }
            } else {
                 printf("- ");
            }
        }
        printf("\nChosen max value: %d (from row %d)\n", currentMax, maxRowIndex);


        if (maxRowIndex != -1) {
            matrix[maxRowIndex][0] = -1;
        }


        if (maxRowIndex != -1 && isArrayEmpty(matrix[maxRowIndex], maxCols) == 1)
        {
            extinctionOrder[extinctionIndex] = maxRowIndex;
            extinctionIndex++;
        }

        totalSize--;

        if (maxRowIndex != -1) {
             buildMaxHeap(matrix[maxRowIndex], maxCols);
        }


        printf("\n\nMatrix after removing max element and re-heapifying row %d:\n\n", maxRowIndex);
        printMatrix(matrix, numRows, maxCols);
    }

    printf("\n\nOrder of extinction (row index + 1): \n\n");
    for (i = 0; i < numRows; i++)
    {
        printf("%d ", extinctionOrder[i] + 1);
    }
    printf("\n");

    free(extinctionOrder);
    free(rowSizes);
    free(randomNumbers);
    for (i = 0; i < numRows; i++)
    {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}
