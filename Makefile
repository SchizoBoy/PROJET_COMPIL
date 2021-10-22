# TOUT POUR LA TABLE LEXICOGRAPHIQUE
SRC_DIR_TABLEX := TabLexico/src
OBJ_DIR_TABLEX := TabLexico/obj
EXE_TABLEX := $(BIN_DIR_TABLEX)/TabLexico_test

# TOUT POUT LA TABLE DES DECLARATION
SRC_DIR_TABDECLA := TabDecla/src
OBJ_DIR_TABDECLA := TabDecla/obj
EXE_TABDECLA := $(BIN_DIR_TABDECLA)/TabDecla_test

# TOUT POUT LA TABLE DE REPRESENTATION DES TYPES
SRC_DIR_TABTYPES := TabRepresentation/src
OBJ_DIR_TABTYPES := TabRepresentation/obj
EXE_TABTYPES := $(BIN_DIR_TABTYPES)/TabRepresentation_test

# TOUT POUT LA TABLE DES REGION
SRC_DIR_TABDECL := tabdecl/src
OBJ_DIR_TABDECL := tabdecl/obj
EXE_REGION := $(BIN_DIR_REGION)/TabRegion_test

# TOUT POUR LES ARBRES
SRC_DIR_ARBRES := arbres/src
OBJ_DIR_ARBRES := arbres/obj
EXE_ARBRES := $(BIN_DIR_ARBRES)/arbres_test

# TOUT POUR LE COMPILATEUR
SRC_DIR_COMPIL := src
OBJ_DIR_COMPIL := obj
BIN_DIR_COMPIL := bin
EXE_COMPIL := $(BIN_DIR_COMPIL)/cpyrr

# TOUS LES EXECUTABLES (DONT CEUX DE TEST)
ALL_EXE  := $(EXE_COMPIL)

SRC_TABLEX := $(wildcard $(SRC_DIR_TABLEX)/*.c)
OBJ_TABLEX := $(SRC_TABLEX:$(SRC_DIR_TABLEX)/%.c=$(OBJ_DIR_TABLEX)/%.o)

SRC_TABDECLA := $(wildcard $(SRC_DIR_TABDECLA)/*.c)
OBJ_TABDECLA := $(SRC_TABDECLA:$(SRC_DIR_TABDECLA)/%.c=$(OBJ_DIR_TABDECLA)/%.o)

SRC_TABTYPES := $(wildcard $(SRC_DIR_TABTYPES)/*.c)
OBJ_TABTYPES := $(SRC_TABTYPES:$(SRC_DIR_TABTYPES)/%.c=$(OBJ_DIR_TABTYPES)/%.o)

SRC_REGION := $(wildcard $(SRC_DIR_REGION)/*.c)
OBJ_REGION := $(SRC_REGION:$(SRC_DIR_REGION)/%.c=$(OBJ_DIR_REGION)/%.o)

SRC_TABDECLA := $(wildcard $(SRC_DIR_TABDECLA)/*.c)
OBJ_TABDECLA := $(SRC_TABDECLA:$(SRC_DIR_TABDECLA)/%.c=$(OBJ_DIR_TABDECLA)/%.o)
OBJ_TABDECLA := $(OBJ_TABDECLA) $(OBJ_DIR_TABLEX)/TabLexico.o

SRC_ARBRES := $(wildcard $(SRC_DIR_ARBRES)/*.c)
OBJ_ARBRES := $(SRC_ARBRES:$(SRC_DIR_ARBRES)/%.c=$(OBJ_DIR_ARBRES)/%.o)
OBJ_ARBRES := $(OBJ_ARBRES) $(OBJ_DIR_TABLEX)/TabLexico.o

SRC_COMPIL := $(SRC_DIR_COMPIL)/*
OBJ_COMPIL := $(OBJ_DIR_COMPIL)/*.o
OBJ_COMPIL_EXT := $(OBJ_DIR_TABLEX)/TabLexico.o
OBJ_COMPIL_EXT := $(OBJ_COMPIL_EXT) $(OBJ_DIR_TABDECLA)/TabDecla.o
OBJ_COMPIL_EXT := $(OBJ_COMPIL_EXT) $(OBJ_DIR_ARBRES)/arbres.o
OBJ_COMPIL_EXT := $(OBJ_COMPIL_EXT) $(OBJ_DIR_TABTYPES)/TabRepresentation.o

CC := gcc
CPPFLAGS := -Iinc -MMD -MP
CFLAGS   := -Wall -pedantic -g -O0
LDFLAGS  :=
LDLIBS   := -lm

.PHONY: all clean

all: $(ALL_EXE)

################################ EXECUTABLES ###################################

# COMPILATION DE L'EXECUTABLE DU COMPILATEUR CPYRR
$(EXE_COMPIL): obj bin $(SRC_COMPIL) $(OBJ_COMPIL_EXT)
	$(CC) $(CFLAGS) -c $(SRC_DIR_COMPIL)/fct_aux_lex.c
	$(CC) $(CFLAGS) -c $(SRC_DIR_COMPIL)/fct_aux_yacc.c
	mv fct_aux* $(OBJ_DIR_COMPIL)
	bison -dv $(SRC_DIR_COMPIL)/cpyrr.y
	mv cpyrr.tab.* cpyrr.output $(OBJ_DIR_COMPIL)
	flex $(SRC_DIR_COMPIL)/cpyrr.l
	mv lex.yy.c $(OBJ_DIR_COMPIL)
	gcc -o $(OBJ_DIR_COMPIL)/lex.yy.o -c $(OBJ_DIR_COMPIL)/lex.yy.c
	gcc -o $(OBJ_DIR_COMPIL)/cpyrr.tab.o -c $(OBJ_DIR_COMPIL)/cpyrr.tab.c
	gcc -o $(EXE_COMPIL) $(OBJ_COMPIL) $(OBJ_COMPIL_EXT)

# COMPILATION DE L'EXECUTABLE DE TEST DE LA TABLE LEXICOGRAPHIQUE
$(EXE_TABLEX): $(OBJ_TABLEX) | $(BIN_DIR_TABLEX)
	$(CC) $(CFLAGS) $^ -o $@

########################## COMPILATION DES SOURCES #############################
$(OBJ_DIR_TABLEX)/%.o: $(SRC_DIR_TABLEX)/%.c | $(OBJ_DIR_TABLEX)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_TABDECLA)/%.o: $(SRC_DIR_TABDECLA)/%.c | $(OBJ_DIR_TABDECLA)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_TABTYPES)/%.o: $(SRC_DIR_TABTYPES)/%.c | $(OBJ_DIR_TABTYPES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

#$(OBJ_DIR_TABREGION)/%.o: $(SRC_DIR_TABREGION)/%.c | $(OBJ_DIR_TABREGION)
#	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_ARBRES)/%.o: $(SRC_DIR_ARBRES)/%.c | $(OBJ_DIR_ARBRES)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


########################### CREATION DES DOSSIERS ##############################
$(OBJ_DIR_TABLEX) $(OBJ_DIR_TABTYPES) $(OBJ_DIR_ARBRES) $(OBJ_DIR_TABDECLA):
	mkdir -p $@
#$(OBJ_DIR_TABREGION):
	mkdir -p $@
$(OBJ_DIR_COMPIL) $(BIN_DIR_COMPIL):
	mkdir -p $@

################################# NETTOYAGE ####################################
clean:
	@$(RM) -rv $(OBJ_DIR_TABLEX) $(OBJ_DIR_TABDECLA) $(OBJ_DIR_COMPIL)
	#@$(RM) -rv $(OBJ_DIR_TABREGION) $(BIN_DIR_TABREGION)
	@$(RM) -rv $(OBJ_DIR_TABTYPES) $(OBJ_DIR_ARBRES)

############################# NETTOYAGE COMPLET ################################
clean_all:
	@$(RM) -rv $(OBJ_DIR_TABLEX) $(OBJ_DIR_TABTYPES) $(OBJ_DIR_ARBRES)
	@$(RM) -rv $(OBJ_DIR_TABDECLA) $(BIN_DIR_COMPIL) $(OBJ_DIR_COMPIL)
	#@$(RM) -rv $(OBJ_DIR_TABREGION) $(BIN_DIR_TABREGION)

-include $(OBJ_TABLEX:.o=.d)
-include $(OBJ_TABDECLA:.o=.d)
-include $(OBJ_TABTYPES:.o=.d)
#-include $(OBJ_TABREGION:.o=.d)
-include $(OBJ_ARBRES:.o=.d)
