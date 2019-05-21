//////////////////////////////////////////////////////////////
///// ESTRUCTURAS DISCRETAS PROYECTO 2 NOTACION POLACA //////
////////// REALIZADO POR ESPINOZA CERON B. ARIAN ///////////
///////////// FACULTAD DE INGENIERIA, UNAM 2019 /////////// 
//////////////////////////////////////////////////////////


/* Como funciona: 
Se tiene que ingresar la expresion en el main, realiza la notacion polaca con
numeros y enteros, pero se tiene que ingresar manual, tambien calcula el resultado
de una expresion que contiene solo enteros, pero se tiene que ingresar con espacios
y sin parentesis (No pude encontrar una funcion que recorriera el array y comparara 
con las expresiones matematicas de suma, resta etc,.. me marcaba errores, pero si se ingresa 
con estas condiciones lo calcula bien)*/


#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <math.h>
#include <ctype.h>


// Funcion principal de la pila, se declaran atributos necesarios y basicos 
struct Stack 
{ 
	int top; 
	unsigned capacity; 
	int* array; 
}; 

// Se crean funciones basicas de la pila
struct Stack* createStack( unsigned capacity ) 
{ 
	struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack)); // el tamaño es dinamico

	if (!stack) // se comprueba si esta vacia
		return NULL; 

	stack->top = -1; 
	stack->capacity = capacity; // se asigna capacidad

	stack->array = (int*) malloc(stack->capacity * sizeof(int));  

	if (!stack->array) // 
		return NULL; 
	return stack; 
} 
int isEmpty(struct Stack* stack) // funcion que comprueba si la pila esta vacia
{ 
	return stack->top == -1 ; 
} 
char jump(struct Stack* stack) //Esta funcion reccore la pila, se utiliza en otras funciones para comprobar si existe algo en especifico
{ 
	return stack->array[stack->top]; 
} 
char pop(struct Stack* stack) //limpia la pila
{ 
	if (!isEmpty(stack)) 
		return stack->array[stack->top--] ; 
	return '$'; 
} 
void push(struct Stack* stack, char op) //agrega a la pila
{ 
	stack->array[++stack->top] = op; 
} 



// Funcion para saber si es caracter o numero 
int isOperand(char ch) 
{ 
	//Se establece que solo se pueden ingresar letras de la A a la Z y numeros del 0 al 9, esta funcion la saque del Cormen
	return (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || (ch>= 'A' && ch <= 'Z'); 
} 



// Funcion para fijar niveles de prioridad 
int Jerarquia(char ch) 
{ 
	switch (ch) 
	{ // el + y el - tienen el mismo nivel de prioridad, en este caso es 1
	case '+': 
	case '-': 
		return 1; 
	// el * y el / tienen el mismo nivel de prioridad, en este caso es 2
	case '*': 
	case '/': 
		return 2; 

	case '^': 
		return 3; 
	} 
	return -1; 
} 

// Algoritmo de conversion a notacion polaca 
int polishNotation(char* exp) 
{ 
	int i, k; 

	
	//crear la pila con dimension de tamaño establecido
	struct Stack* stack = createStack(strlen(exp)); 
	if(!stack) // Se comprueba que la pila se haya creado correctamente
		return -1 ; 

	for (i = 0, k = -1; exp[i]; ++i) 
	{ 
		// Si el caracter leido es valido se  agrega a la pila 
		if (isOperand(exp[i])) 
			exp[++k] = exp[i]; 
		
		// Si es caracter y encuentra ‘(‘, hacer push a la pila 
		else if (exp[i] == '(') 
			push(stack, exp[i]); 

		//funciones para los parentesis
		// Si ecuentra un '(' push a la pila 
		else if (exp[i] == ')') 
		{ 
			while (!isEmpty(stack) && jump(stack) != '(')//recorre con el jump hasta encontrar otro operando 
				exp[++k] = pop(stack); 
			if (!isEmpty(stack) && jump(stack) != '(') 
				return -1; 			 
			else
				pop(stack); 
		} 
		else // si encuentra numero u operano 
		{ 
			while (!isEmpty(stack) && Jerarquia(exp[i]) <= Jerarquia(jump(stack))) 
				exp[++k] = pop(stack); 
			push(stack, exp[i]); 
		} 

	} 

	// vaciar la pila
	while (!isEmpty(stack)) 
	
		exp[++k] = pop(stack ); 

	exp[++k] = '\0';//recorre la expresion para imprimir
	printf(" "); 
	printf( "%s", exp ); //imprime la expresion
} 



/////////////////////////////////////////////////////
/////// FUNCIONES DE CALCULADORA DE ENTEROS /////////
/////////////////////////////////////////////////////

#define die(msg) fprintf(stderr, msg"\n"), abort();//funcion que saque del cormen para poder leer mayusculas y minusculas 
double get(const char *s, const char *e, char **new_e)
{
        const char *t;//creacion de array
        double a, b;//variables temporales para hacer comparaciones

        for (e--; e >= s && isspace(*e); e--);//esta funcion me ayudo mucho porque lee espacios
        for (t = e; t > s && !isspace(t[-1]); t--);//funciona como identificador de operandos
	//si encuentra un espacio es un operando diferente, por eso se tiene que agregar con espacios

        if (t < s) die("underflow");//si el array s es mayor que nuestro array creado t, se desborda

#define get2(expr) b = get(s, t, (char **)&t), a = get(s, t, (char **)&t), a = expr//se pasan los valores a 'b' y a 'a' para poder realizar operaciones
        a = strtod(t, (char **)&e);//cambia un caracter por un flotante
        if (e <= t) {
                if      (t[0] == '+') get2(a + b);//se establecen las operaciones
                else if (t[0] == '-') get2(a - b);
                else if (t[0] == '*') get2(a * b);
                else if (t[0] == '/') get2(a / b);
                else if (t[0] == '^') get2(pow(a, b));
                else {
                        fprintf(stderr, "'%c': ", t[0]);//se imprime el array, una vez terminado
                        die("unknown token");
                }
        }


#undef get2

        *(const char **)new_e = t;//crea un array temporal, pero en realidad no se usa
        return a;
}

double rpn(char *s)//ya que se hicieron operaciones esta funcion se encarga de imprimirlas
{
        char *e = s + strlen(s);//hace el parseo
        double v = get(s, e, (char**)&e);//la  varibale v guardara el resultado de las operaciones

        while (e > s && isspace(e[-1])) e--;//comprueba los espacios de nuevo
        if (e == s) return v;//si ya no hay mas que comparar retorna el resultado

        fprintf(stderr, "\"%.*s\": ", e - s, s);//imprime el resultado
        die("front garbage");
}


int main() { 
	//Aqui se mete la cadena o serie de numeros a ordenar
	printf("\n");
	printf("\nNotacion Polaca: ");
	//TAMBIEN SE PUEDEN AGREGAR LETRAS, AQUI NO IMPORTA SI ESTAS SEPARADAS O NO
	char exp[] = "(4*7+5-2)+(7/8*9-6)*2"; 
	printf("\n");
	polishNotation(exp);
	printf("\n\n");
	//INGRESAR EXPRESION DE ENTEROS A CALCULAR
	//LA EXPRESION SE TIENE QUE INGRESAR SEPARADA POR UN ESPACIO
	//Y SIN PARENTESIS, NO ENCONTRE FUNCIONES QUE ME AYUDARAN
	//A RECORRER ARRAY E IMPRIMIR CON UN ESPACIO
	//PERO LA FUNCION CALCULA BIEN CUALQUIER EXPRESION CON LAS CONDICIONES 
	//ANTES MENCIONADAS
	printf("\n");
	printf("\nResultado en enteros (si es el caso):\n");
	//AQUI SE AGREGA LA EXPRESION EN ENTEROS A CALCULAR, SINO COMENTARLA PARA QUE NO HAGA NADA
	printf("%g\n", rpn(" 4 7 * 5 + 2 - 7 8 / 9 * 6 - 2 * + "));
	return 0;
	
} 

//FIN :)
