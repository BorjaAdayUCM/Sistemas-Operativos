// ./tortuga.sh 256 256 0 code.java

public static final int NIVEL = 5;
public static final float colorInicial = (float)0.7;
public static final float difColor = (float)0.12;

double radioCirculos(int numCirculos, double tam) {
    double radio;
    switch(numCirculos) {
        case 6: radio = tam / 3; break;
        case 5: radio = tam / 2.701; break;
        case 4: radio = tam / (1 + Math.sqrt(2)); break;
        case 3: radio = tam / (1 + (2 / Math.sqrt(3))); break;
        case 2: radio = tam / 2; break;
        default: radio = 1; break;
    }
    return radio;
}

void dibujaCirculoConLineas(double tamano) {
    for(int i = 0; i < 36; i++) {
        advance((int)tamano);
        rotate (180);
        advanceAndPaint((int)tamano);
        rotate(190);
    }
}

void dibuja(double tamano, int nivel, int numCirculos, float color) {
    setColor(color, color, color);
    if(nivel == 0) {
        dibujaCirculoConLineas(tamano);
        return;
    }
    dibujaCirculoConLineas(tamano);
    double tam = radioCirculos(numCirculos, tamano);
    for(int i = 0; i < numCirculos; i++){
	    advance(tamano - tam);
	    dibuja(tam, nivel - 1, numCirculos - 1, color - difColor);
	    rotate(180);
	    advance(tamano - tam);
	    rotate(180 + (360 / numCirculos));
    }
}


public void paint() {
   int numCiculos = 6;
   dibuja(256, NIVEL, numCiculos, colorInicial);
}

