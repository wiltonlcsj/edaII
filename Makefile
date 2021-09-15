run:
	gcc manipula.c -o manipula
	./manipula

run-test:
	gcc manipula.c -o manipula
	./manipula < ./in/1.in > 1-p.out
	cmp ./out/1.out 1-p.out
	rm 1-p.out
	echo "SUCESSO"

run-test-2:
	gcc manipula.c -o manipula
	./manipula < ./in/2.in > 2-p.out
	cmp ./out/2.out 2-p.out
	rm 2-p.out
	echo "SUCESSO"