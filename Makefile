run:
	gcc manipula.c -o manipula && ./manipula

run-test:
	gcc manipula.c -o manipula && ./manipula < 1.in

run-test-2:
	gcc manipula.c -o manipula && ./manipula < 2.in