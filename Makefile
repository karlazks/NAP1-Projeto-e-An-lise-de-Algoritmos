CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11
BIN     = bin
SRC     = src
RES     = results
FIG     = figures

all: $(BIN)/primos $(BIN)/hash $(BIN)/grafos

$(BIN)/primos: $(SRC)/primos.c | $(BIN)
	$(CC) $(CFLAGS) -o $@ $<
$(BIN)/hash: $(SRC)/hash.c | $(BIN)
	$(CC) $(CFLAGS) -o $@ $<
$(BIN)/grafos: $(SRC)/grafos.c | $(BIN)
	$(CC) $(CFLAGS) -o $@ $<
$(BIN):
	mkdir -p $(BIN)

bench: all
	mkdir -p $(RES)
	./$(BIN)/primos --bench > $(RES)/primos.csv
	./$(BIN)/hash   --bench > $(RES)/hash.csv
	./$(BIN)/grafos --media 100 > $(RES)/grafos.csv
	@echo "CSVs em $(RES)/"

figuras:
	mkdir -p $(FIG)
	cd scripts && python3 graficos.py

relatorio:
	cd docs && pdflatex relatorio.tex && pdflatex relatorio.tex

clean:
	rm -f $(BIN)/primos $(BIN)/hash $(BIN)/grafos
	rm -f $(RES)/*.csv $(FIG)/*.png
	rm -f docs/*.aux docs/*.log docs/*.out

.PHONY: all bench figuras relatorio clean
