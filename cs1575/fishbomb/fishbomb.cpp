//Name:       Timothy Stokes
//Section:    102

#include <iostream>

using namespace std;

//Grid functions. Everything to do with the 2D Array
void gridInput(int** arrayPointer, int maxWidth, int maxHeight);
void gridPrint(int** arrayPointer, int maxWidth, int maxHeight);
void gridDelete(int** arrayPointer, int maxWidth, int maxHeight);

//Summation functions. Everything to do with the summation algorithm
int sumUp(int** arrayPointer, int maxHeight, int height, int width);
int sumDown(int** arrayPointer, int maxHeight, int height, int width);
int sumLeft(int** arrayPointer, int maxWidth, int height, int width);
int sumRight(int** arrayPointer, int maxWidth, int height, int width);
int sumBomb(int** arrayPointer, int maxWidth, int maxHeight, int height, int width);

//Optimal Summation function.
void findOptimalBomb(int** arrayPointer, int maxWidth, int maxHeight, int numGrid);

int main()
{
    int** arrayPointer = NULL;

    int count = 0;
    int numGrid = 0;
    int height = 0;
    int width = 0;


    //cout << "Enter the number of grids to input: ";
    cin >> numGrid;

    while (count < numGrid) 
    {
        //cout << "Enter the height of grid #" << count << " :";
        cin >> height;

        //cout << "Enter the width of grid #" << count << " :";
        cin >> width;

        //creates the 2D Dynamic Array
        arrayPointer = new int* [height];

        for (int i = 0; i < height; i++)
        {
            arrayPointer[i] = new int[width];
        } //End Array Creation

        cout << endl;

        //gridCreate(arrayPointer, width, height);

        gridInput(arrayPointer, width, height);

        //gridPrint(arrayPointer, width, height);

        //gridDelete(arrayPointer, width, height);

        findOptimalBomb(arrayPointer, width, height, count);

        //Deletes the 2D Dynamic Array
        for (int i = 0; i < height; i++)
        {
            delete[] arrayPointer[i];
        }
        delete[] arrayPointer;

        arrayPointer = nullptr;
        //End Deletion

        count++;
    }
    return 0;
}

void gridInput(int** arrayPointer, int maxWidth, int maxHeight)
{
    int tmp;
    for (int i = 0; i < maxHeight; i++)
    {
        for (int k = 0; k < maxWidth; k++)
        {
            //cout << "Enter the value for position (" << i << " " << k << ") :";
            cin >> tmp;
                
            arrayPointer[i][k] = tmp;
        }
    }
}

void gridPrint(int** arrayPointer, int maxWidth, int maxHeight)
{
    for (int i = 0; i < maxHeight; i++)
    {
        for (int k = 0; k < maxWidth; k++)
        {
            cout << arrayPointer[i][k] << " ";
        }
        cout << endl;
    }
}

void gridDelete(int** arrayPointer, int maxWidth, int maxHeight)
{
    for (int i = 0; i < maxHeight; i++)
    {
        delete[] arrayPointer[i];
    }
    delete[] arrayPointer;

    arrayPointer = nullptr;
}

int sumUp(int** arrayPointer, int maxHeight, int height, int width)
{
    const int OUTOFBOUNDS = 0;
    int sum = 0;

    if (height > OUTOFBOUNDS)
    {
        for (int i = 0; i < height; i++)
        {
            sum += arrayPointer[i][width];
            
            //Debug
            //cout << "Sum Up, Current sum: " << sum << " at position: " << i << ", "<< width << ")" << endl;
        }

        return sum;
    }

    return sum;
}

int sumDown(int** arrayPointer, int maxHeight, int height, int width)
{
    const int OUTOFBOUNDS = maxHeight;
    int offsetHeight = height + 1; //Offsets height, so the intersection is not included.
    int sum = 0;

    if (offsetHeight < OUTOFBOUNDS)
    {
        for (int i = offsetHeight; i < maxHeight; i++)
        {
            sum += arrayPointer[i][width];
            
            //Debug
            //cout << "Sum Down, Current sum: " << sum << " at position: " << i << ", " << width << ")" << endl;
        }
        return sum;
    }

    return sum;
}

int sumLeft(int** arrayPointer, int maxWidth, int height, int width)
{
    const int OUTOFBOUNDS = 0;
    int sum = 0;

    if (width > OUTOFBOUNDS)
    {
        for (int k = 0; k < width; k++)
        {
            sum += arrayPointer[height][k];
            
            //Debug
            //cout << "Sum Left, Current sum: " << sum << " at position: " << height << ", " << k << ")" << endl;
        }
        return sum;
    }

    return sum;
}

int sumRight(int** arrayPointer, int maxWidth, int height, int width)
{
    const int OUTOFBOUNDS = maxWidth;
    int offsetWidth = width + 1; //offsets width, so the intersection is not included
    int sum = 0;

    if (offsetWidth < OUTOFBOUNDS)
    {
        for (int k = offsetWidth; k < maxWidth; k++)
        {
            sum += arrayPointer[height][k];
            
            //Debug
            //cout << "Sum Right, Current sum: " << sum << " at position: " << height << ", " << k << ")" << endl;
        }
        return sum;
    }

    return sum;
}

int sumBomb(int** arrayPointer, int maxWidth, int maxHeight, int height, int width)
{
    int sum = 0;

    sum += arrayPointer[height][width];

    //Debug
    //cout << "Sum Bomb, Current sum[Intersection]: " << sum << " at position: " << height << ", " << width << ")" << endl;

    sum += sumUp(arrayPointer, maxHeight, height, width);

    //Debug
    //cout << "Sum Bomb, Current sum[Up]: " << sum << " at position: " << height << ", " << width << ")" << endl;
    
    sum += sumDown(arrayPointer, maxHeight, height, width);

    //Debug
    //cout << "Sum Bomb, Current sum[Down]: " << sum << " at position: " << height << ", " << width << ")" << endl;
    
    sum += sumLeft(arrayPointer, maxWidth, height, width);

    //Debug
    //cout << "Sum Bomb, Current sum[Left]: " << sum << " at position: " << height << ", " << width << ")" << endl;
    
    sum += sumRight(arrayPointer, maxWidth, height, width);

    //Debug
    //cout << "Sum Bomb, Current sum[Right, Aka Total]: " << sum << " at position: " << height << ", " << width << ")" << endl;

    return sum;
}

/*int compareSum(int first, int second)
{
    if (first < second)
    {
        return second;
    }
    else
    {
        return first;
    }
}*/

void findOptimalBomb(int** arrayPointer, int maxWidth, int maxHeight, int count)
{
    const int GRID_NUM_OUT = count;
    int currentRow = 0;
    int currentCol = 0;
    int row = 0;
    int col = 0;
    int currentBomb = 0;
    int greatestBomb = 0;

    //Search grid for best sum
    for (int i = 0; i < maxHeight; i++)
    {
        currentRow = i;
        for (int k = 0; k < maxWidth; k++)
        {
            currentBomb = sumBomb(arrayPointer, maxWidth, maxHeight, i, k);
            currentCol = k;
            
            //Debug
            //cout << "Optimal Bomb, Current sum: " << currentBomb << " at position: " << currentRow << ", " << currentCol << ")" << endl;
            if (greatestBomb < currentBomb)
            {
                greatestBomb = currentBomb;
                row = currentRow;
                col = currentCol;
            }
            //Debug
            //cout << "Optimal Bomb, Best Sum: " << greatestBomb << " at position: " << row << ", " << col << ")" << endl;
        }
    }
    
    cout << "#" << GRID_NUM_OUT << ": at (" << row << "," << col
        << ") Bender catches " << greatestBomb << " fish." << "\n";
}