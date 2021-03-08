// ./tortuga.sh 128 256 0 code.java

public static final int nivel = 12;

void dibujaCurvas(int nivel, double l, double r, double g, double b) {
    float rojo = (float) r;
    float verde = (float) g;
    float azul = (float) b;
    setColor(rojo, verde, azul);
    if (nivel == 0) {
        advanceAndPaint(l);
        return;
    }
        double base = l / Math.sqrt(2);
        rotate(45);
        dibujaCurvas(nivel - 1, base, r + 0.08, g - 0.2, b + 0.16);
        rotate(270);
        dibujaCurvas(nivel - 1, base, r - 0.08, g + 0.2, b - 0.16);
        rotate(45);
}

public void paint() {
   dibujaCurvas(nivel, 256, 0, 0, 0);
}
