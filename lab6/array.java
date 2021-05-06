class SharedVector {
	private int[] vector;
	public int length;

	// Inicia e imprime o vetor
	public SharedVector(int size){
		this.vector = new int[size];
		this.length = size;
		
		System.out.print("Inicializando Array => [ ");
		for(int i = 0; i < vector.length; i++){
			this.vector[i] = i;
			System.out.print(i + " ");
		}
		System.out.print("]\n\n");
	}

	// Incrementa o valor em 1
	public void incrementIndex(int index){
		this.vector[index]++;
	}

	// Pega valor do vetor
	public int getIndex(int index){
		return this.vector[index];
	}
}

class ArrThread extends Thread{
	private int id;
	private int step;
	private boolean isLast; // É a thread a operar na parte final do array?
	SharedVector vector;
	 
	// Inicializa valores da Thread
	public ArrThread(int id, SharedVector v, int step, boolean isLast){
		this.id = id;
		this.vector = v;
		this.step = step;
		this.isLast = isLast;
	}

	// Realiza as Operações
	public void run(){
		int start = this.step * this.id; // Indice a Começar as operações
		int end = this.isLast ? vector.length : start + this.step; // Termino das Operações

		System.out.println("Thread " + id + " Iniciando em " + start + " e finalizando em " + end);
		
		for(int i = start; i < end; i++){
			vector.incrementIndex(i);
		}
	}
}

class IncrementoArray {
	static final int N = 4; // Numero de Threads


	public static void main(String[] args){
		Thread[] threads = new Thread[N];
		SharedVector vector = new SharedVector(10);

		// Cria as Threads
		for (int i=0; i<threads.length; i++) {
			threads[i] = new ArrThread(i, vector, vector.length/N, i == N-1);
		}

		// Inicia as Threads
		for (int i=0; i<threads.length; i++) {
			threads[i].start();
		}

		// Aguarda o fim das Threads
		for (int i=0; i<threads.length; i++) {
			try { threads[i].join(); } catch (InterruptedException e) { return; }
		}

		// Vefica o resultado
		System.out.print("\nArray Final => [ ");
		for(int i = 0; i < vector.length; i++){

			System.out.print(vector.getIndex(i) + " ");

			if(vector.getIndex(i) != i + 1){
				System.out.println("\n\nVALOR INCORRETO NO INDICE " + i);
				System.exit(0);
			}	
		}
		System.out.print("]\n\n");

		System.out.println("tudo certo");
	}
}	
