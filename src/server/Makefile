#As minhas sources...
SRC=server.c auxfuncs.c

#O binário executável (nome do programa)
PROG=rasp

################################

CC=g++
CFLAGS=-Wall

OBJ=$(SRC:.c=.o)  #macro automática para gerar os OBJ a partir das SRC

INCLUDE=    #para as header files (cabeçalhos)
#LIB=-lm        #bibliotecas específicas a usar //lib.m
LIB+=`pkg-config --libs opencv`

DOXYFILE=Doxyfile
AULA = 5
############################################
# Regras
############################################

###########################################
# Regras relativas à compilação e linkagem

$(PROG): prototypes.h $(OBJ)
	$(CC) $(OBJ) -o $(PROG) $(LIB)      

.c.o:
	$(CC) $(CFLAGS) -c $(INCLUDE) $< -o $@

######################################################################

proto prototypes.h: $(SRC)   #decidir se esta dependência deve ou não existir!
	@echo "Rebuild prototypes.h"
	@echo '/*File generated automatically. Do not edit*/' > prototypes.h
	@echo '/*Generated on' `hostname` ',' `date` '*/' >> prototypes.h
	@cproto -q -O /dev/null $(SRC) >> prototypes.h

#########################################
# Regras relativas à gestão e organização

clean:
	rm -rf $(PROG) $(OBJ)

allclean: clean
	rm -rf html latex Doxyfile PARIlogo.png

##########################################
# Regras relativas à geração de documentação

#depende da $(DOXYFILE) e do ficheiro index.html
doc: $(DOXYFILE) html/index.html

#A $(DOXYFILE) só depende de si própria.
#Se não existir é criada e alguns defaults são ajustados:
$(DOXYFILE):
	doxygen -g $(DOXYFILE) ; \
	sed -i 's/^PROJECT_NAME.*$$/PROJECT_NAME          = "My Project  $(PROG)"/' $(DOXYFILE) ;\
	sed -i 's/^GENERATE_LATEX.*$$/GENERATE_LATEX      = NO/'                   $(DOXYFILE) ;\
	sed -i 's/^HAVE_DOT.*$$/HAVE_DOT                  = YES/'                  $(DOXYFILE) ;\
	sed -i 's/^SOURCE_BROWSER.*$$/SOURCE_BROWSER      = YES/'                  $(DOXYFILE) ;\
	sed -i 's/^GENERATE_TREEVIEW.*$$/GENERATE_TREEVIEW= YES/'                  $(DOXYFILE) ;\
	sed -i 's/^PROJECT_LOGO.*$$/PROJECT_LOGO          = PARIlogo.png/'         $(DOXYFILE) ;

#O ficheiro index.html depende obviamente da Doxyfile e das SRC, mas
#pode ter outras dependências adicionais...
html/index.html: $(DOXYFILE) $(SRC) mainpage.dox PARIlogo.png
	doxygen

#Forçar a geração de documentação mesmo que ela já exista (não depende de index.html)
forcedoc: $(DOXYFILE)
	doxygen

#Gera ficheiro de logotipo automaticamente
logo PARIlogo.png:
	convert pattern:circles \( +clone \) +append \( +clone \) -append \
		-fill grey -opaque black  -blur 0x0.5 -shade 120x45 \
		-gravity center -crop 50%  +repage    miff:- |\
		convert  -size 80x80 tile:- -fill green \
		-draw "scale 2.5,3 text 2,12 PARI" \
		-draw "scale 2,2 text 2,32 'Aula $(AULA)'" PARIlogo.png

##################################################
view: doc
	firefox html/index.html

###########################################
# targets que não são ficheiros e evita
# eventuais conflitos se porventura existirem
# ficheiros ou diretorios com estes nomes.
#
.PHONY: doc clean allclean forcedoc


