int arr[3];

void foo(int arr[]) {
	int i;
	for (i = 0; i < 3; i++) {
	  printf("i = %i\n", i);
	  arr[i] = i;
	}
}
int main() {
foo(arr);
	return 0;
}


