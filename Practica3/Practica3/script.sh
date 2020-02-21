#!/bin/bash

cd schedsim/examples

echo "Que fichero se desea simular?"
read fich
while [ ! -f $fich ]; do
	echo "Fichero no encontrado, introduzca otro"
	read fich
done 

cd ../..

echo "Numero maximo de CPUs?"
read num
while [ $num -gt 8 ]; do
	echo "Numero de CPUs no valido, debe se menor igual que 8"
	read num
done 

if [ -d resultados ]; then
	rm -rf resultados
fi
mkdir resultados

for nameSched in RR FCFS SJF PRIO; do
	if [ $nameSched == PRIO ] || [ $nameSched == SJF ]; then
		echo "Usar expropiacion para $nameSched: S/N"
		read expropiacion
		while [[ "$expropiacion" != "S" && "$expropiacion" != "N" ]]; do
			echo "Usar expropiacion para $nameSched: S/N"
			read expropiacion
		done
	fi

	for ((cpus=1; cpus <= num; cpus++)); do
		if [ "$expropiacion" == "S" ]; then
			./schedsim/schedsim -i ./schedsim/examples/$fich -n $cpus -s $nameSched -p
		else
			./schedsim/schedsim -i ./schedsim/examples/$fich -n $cpus -s $nameSched
		fi
		for ((i = 0; i < cpus;i++)); do
			mv CPU_$i.log  resultados/$nameSched-CPU-$i.log
			cd gantt-gplot
			./generate_gantt_chart ../resultados/$nameSched-CPU-$i.log
			cd ..
		done
	done
done