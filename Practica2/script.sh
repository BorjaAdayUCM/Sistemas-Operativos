#Crear directorio de archivos temporales
mkdir tmp

#Copiar archivos en directorio temporal y nuestro FS
cp ./src/fuseLib.c ./mount-point
cp ./src/myFS.h ./mount-point
cp ./src/fuseLib.c ./tmp
cp ./src/myFS.h ./tmp

#Auditoria del disco
./my-fsck virtual-disk

#Comprobacion de que se han copiado bien
if diff -q ./src/fuseLib.c ./tmp/fuseLib.c; then
	if diff -q ./src/myFS.h ./tmp/myFS.h; then
		if diff -q ./src/fuseLib.c ./mount-point/fuseLib.c; then
			if diff -q ./src/myFS.h ./mount-point/myFS.h; then
				echo "Archivos copiados correcamente
				"
			fi
		fi
	fi
fi

#Truncar a un bloque menos
truncate --size -4096 ./tmp/fuseLib.c

truncate --size -4096 ./mount-point/fuseLib.c

#Auditoria del disco
./my-fsck virtual-disk

#Comprobacion de que se ha truncado bien
if diff -q ./src/fuseLib.c ./mount-point/fuseLib.c; then
	echo "El archivo original y el truncado son iguales, algo no va bien
	"
else
	echo "Son diferentes, todo ha ido correctamente
	"
fi

#Copiar un tercer fichero
cp Makefile ./mount-point

#Auditoria del disco
./my-fsck virtual-disk

#Comprobacion de que se ha copiado bien
if diff -q ./Makefile ./mount-point/Makefile; then
	echo "Archivo copiado correcamente
	"
fi

#Truncar a un bloque mas
truncate --size +4096 ./tmp/myFS.h

truncate --size +4096 ./mount-point/myFS.h

#Auditoria del disco
./my-fsck virtual-disk

#Comprobacion de que se ha truncado bien
if diff -q ./src/myFS.h ./mount-point/fuseLib.c; then
	echo "El archivo original y el truncado son iguales, algo no va bien
	"
	else
	echo "Son diferentes, todo ha ido correctamente
	"
fi