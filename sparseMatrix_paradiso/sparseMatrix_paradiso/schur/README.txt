1) full.bin - ����������� ������� A
������:
int Format; 	// 0 - ������������ ������
int nRows;	//����� �����
int nCols;	//����� ��������

int valuesSize; 	//���-�� ��������� �������� � �������
int rowIndexSize;	//���-�� ��������� � ������� �������� �����
int colIndexSize;	//���-�� ��������� � ������� �������� ��������

int colIndexes[colIndexSize];		//������ �������� ��������
int rowIndexes[rowIndexSize];		//������ �������� �����
complex<double> values[valuesSize]; 	//������ ��������� ��������

� ������������ ������� �������� valuesSize, rowIndexSize � colIndexSize ���������.



2) interindex.bin - ������ �������� ��� schur
������
int count;		//���������� ��������
int indeces[count];	//���� ������� (��������� ���������� � 0)



3) schur.bin - ���������� ������� schur
������:
complex<double> values[count*count];
