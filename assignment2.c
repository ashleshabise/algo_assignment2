#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<stdbool.h>

/*data structure for saving a multi variable polynomial*/
typedef struct polynomial{
	int i;
	int j;
	int k;
	double coeff;
	struct polynomial *next;
	}polynomial;


// function to print a given polynomial
void printP(polynomial *p); 
	
/* function to append a new term to the polynomial */
void push(polynomial **p, polynomial *newTerm);
/* To Compute del(p)*/
polynomial* delP(polynomial* p);

/*function to compute delta P to the power k at given x*/
double delPK(polynomial* p,int k, double* x);


/*function for checking a polynomial to be homogeneous*/
bool checkHomoPoly(polynomial* p,int m);

/*function to compute (2,2)k+1 */
int COMPUTE(int k);

double computeQ(int m,polynomial* pin,double*x);

/* function to extract a polynomial as linked list from coeffP array */
polynomial *extractP(int m, int nonZeroTermIndices[],int terms, double *coeffP);

int main() {

	/*input degree of polynomial*/
	int m, iter, index;
	printf("Input Degree of input polynomial : ");
	scanf("%d",&m);
	printf("%d\n", m);
	int terms;
	printf("Enter the number of non-zero terms: ");
	scanf("%d", &terms);
	printf("%d\n", terms);
	int nonZeroTermIndices[terms];
	double coeffP[terms];
	printf("Enter the index and the corresponding coefficient of the polynomial array: \n");
	for ( iter=0; iter<terms; iter++) {
		printf("index: ");
		scanf("%d", &index);
		printf("%d\n", index);
		nonZeroTermIndices[iter] = index;
		printf("p[index] : ");
		scanf("%lf", &coeffP[iter]);
		printf("%lf\n", coeffP[iter]);
		printf("iter = %d \n", iter);
	}
	fflush(stdout);
	polynomial *pin = NULL;
	pin = extractP(m, nonZeroTermIndices, terms, coeffP);

	/* Check that polynomial is homogeneous or not*/
	if(checkHomoPoly(pin,m))
		printf("\nInput polynomial is Homogeneous");
	else
		{
			printf("\nInput Polynomial is not Homogenous");
			return 3;
		}
	
	/*input value of x*/
	double x[3];
	printf("\nInput the value of x: ");
	scanf("%lf%lf%lf",&x[0],&x[1],&x[2]);
	
	/*Compute the polynomial q(x) defined in the problem set at given point x*/
	printf("\nComputing the Polynomial q(x) at given point: \n");
	double value  = computeQ(m, pin, x);
	printf("The value of polynomial q(x) defined in problem set :  %lf \n\n", value);
	
	return 0;
}

// function to print the polynomial 
void printP(polynomial *p) {
	polynomial *temp = p;
	while ( temp) {
		printf("%lf*x^%d*y^%d*z^%d %c ", temp->coeff, temp->i, temp->j, temp->k, temp->next ? '+' : ' ');
		temp = temp->next;
	}
	printf("\b\b\n");
}

void push(polynomial **p, polynomial *newTerm) {
	// get to the last term and attach newTerm there.
	polynomial *temp = *p;
	if (*p == NULL) {
		*p = newTerm;
		return;
	}
	while(temp->next) temp=temp->next;
	temp->next = newTerm;
	
}

/*function for checking a polynomial to be homogeneous*/
bool checkHomoPoly(polynomial* pin,int m) {
	polynomial* curTerm;
	curTerm=pin;
	printf("%d", m);
	while(curTerm != NULL) {
		printf("%d %d %d\n", curTerm->i, curTerm->j, curTerm->k);
		if ( ((curTerm->i) + (curTerm->j) + (curTerm->k)) !=m )
			return false;
		curTerm = curTerm->next;
	}
	return true;
}

polynomial *extractP(int m, int nonZeroTermIndices[],int terms, double *coeffP) {
	fflush(stdout);
	int i,j,k, iter, index;
	polynomial *p = NULL;
	for (iter=0; iter< terms ; iter++) {
		index = nonZeroTermIndices[iter];
		polynomial *newTerm = (polynomial*)malloc(sizeof(polynomial));
		k = index%(m+1);
		j = ((index%(int)pow((m+1),2))-k)/(m+1);
		i =(index-k-j)/(int)pow((m+1),2);
		newTerm->i = i;
		newTerm->j = j;
		newTerm->k = k;
		newTerm->coeff = coeffP[iter];
		newTerm->next = NULL;
		push(&p, newTerm);
	}
	return p;
}

/*function to compute (2,2)k+1 */
/* One Should check that 
(2,2)k+1 = 2.4.6..... (2k+2)*/
int COMPUTE(int k)
{
	int n=1;
	for ( int i=1; i<=k+1; i++)  n *=  2*i;
	return n;
}
/*  Computes del(p) */
polynomial *delP(polynomial *p) {
	polynomial *currentTerm; 	
	polynomial *list1, *list2 , *list3;
	list1 = list2  = list3 = NULL;
	int i, j, k; 
	double coeff;
	fflush(stdout);
	
	for (int iter=0; iter < 3; iter++) {
			currentTerm = p;
		while ( currentTerm != NULL )  {
			i = currentTerm->i;
			j = currentTerm->j;
			k = currentTerm->k;
			coeff = currentTerm->coeff;
			polynomial *newTerm =  (polynomial*)malloc(sizeof(polynomial));
			newTerm->i = i;
			newTerm->j = j;
			newTerm->k = k;
			newTerm->coeff = coeff;
			newTerm->next = NULL;
			switch(iter) {
			// wrt x
				case 0 : if ( i >= 2  ) {
										coeff *= i*(i-1);	
										newTerm->i = i-2;
										newTerm->coeff = coeff;
										// This pushes newTerm at the end of the linked list. 
										push(&list1,newTerm);
									}
								break;
			// wrt y
				case 1 : if ( j >= 2  ) {
										coeff *= j*(j-1);	
										newTerm->j = j-2;
										newTerm->coeff = coeff;
										// This pushes newTerm at the end of the linked list. 
										push(&list2,newTerm);
									}
								break;
			// wrt z
				case 2 : if ( k >= 2  ) {
										coeff *= k*(k-1);	
										newTerm->k = k-2;
										newTerm->coeff = coeff;
										// This pushes newTerm at the end of the linked list. 
										push(&list3,newTerm);
									}
								break;
			}
			currentTerm = currentTerm->next; 
		}
	}
	polynomial *temp = list1;
	
	if (list1)
		while(temp->next) temp = temp->next;
	else {
		if (list2) {
			temp = list2;
			while(temp->next)  temp = temp->next;
			temp->next = list3;
			return list2;
		} else {
			return list3;
		}
	}
	temp->next = list2;
	temp = temp->next;
	if(temp)
		while(temp->next) temp = temp->next;

	if(temp)
		temp->next = list3;
	return list1;
}
// Computing polynomial at x 
double polyAtX(polynomial* p, double x[3]) {
	double delpatx=0;
	polynomial *predptr=p;
	while(predptr!=NULL) {
			delpatx=delpatx+(predptr->coeff)*(pow(x[0],predptr->i))*(pow(x[1],predptr->j))*(pow(x[2],predptr->k));
			predptr =  predptr->next;
		}
	return delpatx;
}

double modXSquare(double *x) {
	return x[0]*x[0] + x[1]*x[1]+ x[2]*x[2];
}

// Apply delP K times and return a constant value at  x[]
double delPK(polynomial *p, int k, double *x) {
	polynomial *result = p;
	for ( int i=0; i < k ; i++) result = delP(result);
	
	return  polyAtX(result, x);

}

double computeQ(int m,polynomial* p, double*x) {
	polynomial *q = NULL;
	double sum, numerator, temp, denominator;
	sum = temp = numerator = denominator = 0;
	int sign;

	for ( int iter=0; iter <= m/2 ; iter++ ) {
		printf("Iteration %d : \n", iter+1);
		sign = iter%2 ? -1 : 1;
		printf("sign : %d\n", sign);
		printf("modxsquare^k : %lf\n", pow(modXSquare(x), iter));
		printf("delp k times : %lf\n", delPK(p, iter, x));
		printf("COMPUTE(k): %i\n", COMPUTE(iter));
		printf("pow(3+2m, k):  %lf\n\n", pow(3+2*m, iter+1));
		numerator = pow(modXSquare(x), iter) * delPK(p, iter, x);
		numerator *=  sign;
		denominator =  COMPUTE(iter) *  pow(3+2*m, (iter+1));
		sum += numerator/denominator;
	}
	return sum;
}

