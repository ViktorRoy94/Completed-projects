1) full.bin - разреженная матрица A
Формат:
int Format; 	// 0 - координатный формат
int nRows;	//Число строк
int nCols;	//Число столбцов

int valuesSize; 	//Кол-во ненулевых значений в матрице
int rowIndexSize;	//Кол-во элементов в массиве индексов строк
int colIndexSize;	//Кол-во элементов в массиве индексов столбцов

int colIndexes[colIndexSize];		//Массив индексов столбцов
int rowIndexes[rowIndexSize];		//Массив индексов строк
complex<double> values[valuesSize]; 	//Массив ненулевых значении

В координатном формате значения valuesSize, rowIndexSize и colIndexSize совпадают.



2) interindex.bin - массив индексов для schur
Формат
int count;		//количество индексов
int indeces[count];	//сами индексы (Нумерация начинается с 0)



3) schur.bin - заполненая матрица schur
Формат:
complex<double> values[count*count];
