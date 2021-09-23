run:
	gcc manipula.c -o manipula
	./manipula

run-tests:
	make run-test
	make run-test-2
	make run-test-3

run-test:
	rm arquivo
	gcc manipula.c -o manipula
	./manipula < ./in/1.in > 1-p.out
	cmp ./out/1.out 1-p.out
	rm 1-p.out

run-test-2:
	rm arquivo
	gcc manipula.c -o manipula
	./manipula < ./in/2.in > 2-p.out
	cmp ./out/2.out 2-p.out
	rm 2-p.out

run-test-3:
	rm arquivo
	gcc manipula.c -o manipula
	./manipula < ./in/3.in > 3-p.out
	cmp ./out/3.out 3-p.out
	rm 3-p.out