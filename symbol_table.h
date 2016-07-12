#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500

#define COLUMN 1
#define ENTITY 0

#define PRIMARY_KEY 2
#define FOREIGN_KEY 3

#define FOUND 1
#define NOT_FOUND 0

#define VALID 1
#define INVALID 0

int select_fields_counter = 0;

//Structure for Columns
struct element
{
	char element_name[MAX];
	int element_scope;
	char element_type[MAX];
	struct element *next_element;
	char entity_name[MAX];
	
};

//Structure for Entities
struct entity 
{
  char entity_name[MAX];
  struct element *element;
  struct entity *next_entity;
};

struct select
{
	int selection_fields;
	char entity_name[MAX];
	int validity;
	struct select *next_select;
};

struct selected_fields
{
	char field_name[MAX];
	struct select *select;
	int validity;
	struct selected_fields *next_selected_field;
};

struct foreign_key
{
	char foreign_key[MAX];
	char table[MAX];
	char table_primary_key[MAX];

	struct foreign_key *next_foreign_key; 
};

//Structures
typedef struct element element_instance;
typedef struct entity entity_instance;
typedef struct select select_instance;
typedef struct selected_fields selected_fields_instance;
typedef struct foreign_key foreign_key_instance;


//Pointers
element_instance *element_list_pointer;
entity_instance *entity_list_pointer;
select_instance *select_list_pointer;
selected_fields_instance *selected_fields_list_pointer;
foreign_key_instance *foreign_key_list_pointer;


// Inicialização das listas
element_instance *initialize_element_list(element_instance *list_pointer)
{
  list_pointer = (element_instance*) malloc (sizeof(element_instance));
  list_pointer->next_element = NULL;
  return list_pointer;
}

entity_instance *initialize_entity_list(entity_instance *list_pointer)
{
  list_pointer = (entity_instance*) malloc (sizeof(entity_instance));
  list_pointer->next_entity = NULL;
  return list_pointer;
}

select_instance *initialize_select_list(select_instance *list_pointer)
{
	list_pointer = (select_instance*) malloc (sizeof(select_instance));
	list_pointer->next_select = NULL;
	return list_pointer;
}

foreign_key_instance *initialize_foreign_list(foreign_key_instance *list_pointer)
{
	list_pointer = (foreign_key_instance*) malloc (sizeof(foreign_key_instance));
	list_pointer->next_foreign_key = NULL;
	return list_pointer;
}

selected_fields_instance *initialize_selected_fields_list(selected_fields_instance *list_pointer)
{
	list_pointer = (selected_fields_instance*) malloc (sizeof(selected_fields_instance));
	list_pointer->next_selected_field = NULL;
	return list_pointer;
}


// Declaração das funções da tabela de simbolos
void insert_element(element_instance *list_pointer, char element_name_insert[MAX], int element_scope_insert, char element_type_insert[MAX]);
void insert_entity(entity_instance *list_pointer, element_instance *element_insert);
void insert_select(select_instance *list_pointer, int selection_fields, char entity_name[MAX]);
void insert_foreign_key(foreign_key_instance *list_pointer, char table[MAX], char table_primary_key[MAX], char foreign_key[MAX]);
void insert_selected_fields(selected_fields_instance *list_pointer, char field_name[MAX]);
void create_entity_list(element_instance *list_pointer);
int print_element_list(element_instance *list_pointer);
int is_pk(element_instance *list_pointer, char primary_key[MAX]);
void print_entity_list(entity_instance *list_pointer);
void print_select_list(select_instance *list_pointer);
void print_foreign_key_list(foreign_key_instance *list_pointer);
void print_selected_fields_list(selected_fields_instance *list_pointer);
void associate_select_selected_fields(selected_fields_instance *selected_fields_list_pointer, select_instance *select_list_pointer);
void validate_selected_fields(selected_fields_instance *selected_fields_list_pointer, element_instance *element_list_pointer);

// Declaração das funcões relacionadas a geração de código
char *getPK(element_instance *list_pointer); 

void insert_element(element_instance *list_pointer, char element_name_insert[MAX], int element_scope_insert, char element_type_insert[MAX])
{
	element_instance *new_element, *first_element;
	new_element = (element_instance*) malloc(sizeof(element_instance));
	strcpy(new_element->element_name, element_name_insert);
	new_element->element_scope = element_scope_insert;
	strcpy(new_element->element_type, element_type_insert);
	first_element = list_pointer->next_element;
	list_pointer->next_element = new_element;
	new_element->next_element = first_element;
}

void insert_entity(entity_instance *list_pointer, element_instance *element_insert)
{
  entity_instance *new_entity, *first_entity;
  new_entity = (entity_instance*) malloc(sizeof(entity_instance));
  strcpy(new_entity->entity_name, element_insert->element_name);
  new_entity->element = element_insert;
  first_entity = list_pointer->next_entity;
  list_pointer->next_entity = new_entity;
  new_entity->next_entity = first_entity;
}

void insert_select(select_instance *list_pointer, int selection_fields, char entity_name[MAX])
{
	select_instance *new_select, *first_select;
	new_select = (select_instance*) malloc(sizeof(select_instance));
	new_select->selection_fields = selection_fields;
	new_select->validity = VALID;
	strcpy(new_select->entity_name, entity_name);
	first_select = list_pointer->next_select;
	list_pointer->next_select = new_select;
	new_select->next_select = first_select;
}

void insert_foreign_key(foreign_key_instance *list_pointer, char table[MAX], char table_primary_key[MAX], char foreign_key[MAX])
{
	foreign_key_instance *new_foreign_key, *first_foreign_key;
	new_foreign_key = (foreign_key_instance*) malloc(sizeof(foreign_key_instance));
	strcpy(new_foreign_key->table, table);
	strcpy(new_foreign_key->table_primary_key, table_primary_key);
	strcpy(new_foreign_key->foreign_key, foreign_key);
	first_foreign_key = list_pointer->next_foreign_key;
	list_pointer->next_foreign_key = new_foreign_key;
	new_foreign_key->next_foreign_key = first_foreign_key;
}

void insert_selected_fields(selected_fields_instance *list_pointer,char field_name[MAX])
{
	selected_fields_instance *new_selected_field, *first_selected_field;
	new_selected_field = (selected_fields_instance*) malloc(sizeof(selected_fields_instance));
	strcpy(new_selected_field->field_name, field_name);
	new_selected_field->select = NULL;
	new_selected_field->validity = INVALID;
	first_selected_field = list_pointer->next_selected_field;
	list_pointer->next_selected_field = new_selected_field;
	new_selected_field->next_selected_field = first_selected_field;
}

void create_entity_list(element_instance *list_pointer)
{
	if(list_pointer == NULL)
	{ 
		printf("There is no entity.\n");
		exit(1);
	}
	char entity_name_for_element[MAX];
	element_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer->next_element;
	while(auxiliary_pointer != NULL)
	{		
    	if(auxiliary_pointer->element_scope == 0)
    	{
     		insert_entity(entity_list_pointer, auxiliary_pointer); 
     		strcpy(entity_name_for_element, auxiliary_pointer->element_name);
    	}
    	else{
    		strcpy(auxiliary_pointer->entity_name, entity_name_for_element);
    	}

		auxiliary_pointer = auxiliary_pointer->next_element;
	}
}

int print_element_list(element_instance *list_pointer)
{
	int elements_counter = 0;

	if(list_pointer == NULL)
	{ 
		printf("There is no instance.\n");
		exit(1);
	}

	element_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer->next_element;
	while(auxiliary_pointer != NULL)
	{		
    	printf("%s\t",auxiliary_pointer->element_name);
		printf("%d\t",auxiliary_pointer->element_scope);
		printf("%s\t",auxiliary_pointer->element_type);
		if(auxiliary_pointer->element_scope == COLUMN)
		{
			printf("%s\t",auxiliary_pointer->entity_name);
		}
		printf("\n");
		auxiliary_pointer = auxiliary_pointer->next_element;
		elements_counter += 1;
	}
	return elements_counter;
}

void print_entity_list(entity_instance *list_pointer)
{
	if(list_pointer == NULL)
	{ 
		printf("There is no entity.\n");
		exit(1);
	}

	entity_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer->next_entity;
	while(auxiliary_pointer != NULL)
	{		
    	printf("%s\n",auxiliary_pointer->entity_name);
		auxiliary_pointer = auxiliary_pointer->next_entity;
	}
}

void print_select_list(select_instance *list_pointer)
{
	if(list_pointer == NULL)
	{ 
		printf("There is no select.\n");
		exit(1);
	}

	select_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer->next_select;
	while(list_pointer->next_select != NULL)
	{	
	    int validade_select = search_entity(list_pointer->next_select->entity_name);	
	    if(validade_select == FOUND)
	    {    
	      	printf("Select %d from %s.\n",list_pointer->next_select->selection_fields, list_pointer->next_select->entity_name);
	    }
	    else
	   	{
	   		list_pointer->next_select->validity = INVALID;
	    	printf("Select query doesnt have a valid entity.\n");
	    }
		list_pointer->next_select = list_pointer->next_select->next_select;
	}
	list_pointer->next_select = auxiliary_pointer;
}

void print_foreign_key_list(foreign_key_instance *list_pointer)
{
	if(list_pointer == NULL)
	{ 
		printf("There is no foreign key.\n");
		exit(1);
	}

	foreign_key_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer->next_foreign_key;
	while(auxiliary_pointer != NULL)
	{	
	    printf("FOREIGN_KEY %s REFERENCES %s(%s) \n",auxiliary_pointer->foreign_key, auxiliary_pointer->table, auxiliary_pointer->table_primary_key);

		auxiliary_pointer = auxiliary_pointer->next_foreign_key;
	}
}

void print_selected_fields_list(selected_fields_instance *list_pointer)
{
	if(list_pointer == NULL)
	{ 
		printf("There is no selected fields.\n");
		exit(1);
	}

	selected_fields_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer->next_selected_field;
	while(auxiliary_pointer != NULL)
	{	
		if(auxiliary_pointer->validity == VALID){
    		printf("%s\t", auxiliary_pointer->field_name);
    		printf("%s\n", auxiliary_pointer->select->entity_name);
    	}
    	else
    	{
    		printf("SELECT FIELD %s NOT VALID\n", auxiliary_pointer->field_name);
    	}
		auxiliary_pointer = auxiliary_pointer->next_selected_field;
	}
}

void associate_select_selected_fields(selected_fields_instance *selected_fields_list_pointer, select_instance *select_list_pointer)
{
	select_instance *auxiliary_select_pointer;
	auxiliary_select_pointer = select_list_pointer->next_select;
	selected_fields_instance *auxiliary_selected_fields_pointer;
	auxiliary_selected_fields_pointer = selected_fields_list_pointer->next_selected_field;
	while(select_list_pointer->next_select != NULL)
	{
		int i;
		for(i = 0; i < select_list_pointer->next_select->selection_fields; i++)
		{
			selected_fields_list_pointer->next_selected_field->select = select_list_pointer->next_select;
			selected_fields_list_pointer->next_selected_field = selected_fields_list_pointer->next_selected_field->next_selected_field;
		}
		select_list_pointer->next_select = select_list_pointer->next_select->next_select;
	}
	select_list_pointer->next_select = auxiliary_select_pointer;
	selected_fields_list_pointer->next_selected_field = auxiliary_selected_fields_pointer;
}

void validate_selected_fields(selected_fields_instance *selected_fields_list_pointer, element_instance *element_list_pointer)
{
	element_instance *auxiliary_element_pointer;
	auxiliary_element_pointer = element_list_pointer->next_element;
	selected_fields_instance *auxiliary_selected_fields_pointer;
	auxiliary_selected_fields_pointer = selected_fields_list_pointer->next_selected_field;
	while(selected_fields_list_pointer->next_selected_field != NULL)
	{
		while(auxiliary_element_pointer != NULL)
		{	
			if(strcmp(selected_fields_list_pointer->next_selected_field->field_name, "*") == 0)
			{
				if(strcmp(selected_fields_list_pointer->next_selected_field->select->entity_name, auxiliary_element_pointer->entity_name) == 0)
				{
					selected_fields_list_pointer->next_selected_field->validity = VALID;
					break;
				}
			}
			else
				{
					if(auxiliary_element_pointer->element_scope == COLUMN)
					{
						if(strcmp(selected_fields_list_pointer->next_selected_field->select->entity_name, auxiliary_element_pointer->entity_name) == 0
						&& strcmp(selected_fields_list_pointer->next_selected_field->field_name, auxiliary_element_pointer->element_name) == 0)
						{
							selected_fields_list_pointer->next_selected_field->validity = VALID;
							break;
						}
					}
				}
			auxiliary_element_pointer = auxiliary_element_pointer->next_element;	
		}
		auxiliary_element_pointer = element_list_pointer->next_element;
		selected_fields_list_pointer->next_selected_field = selected_fields_list_pointer->next_selected_field->next_selected_field;
	}
	selected_fields_list_pointer->next_selected_field = auxiliary_selected_fields_pointer;
}


char *getPK(element_instance *list_pointer) 
{

	element_instance *auxiliary_pk;
	auxiliary_pk = list_pointer;
	int m = 0;
	static char primary_key[MAX];
	for (m = 0; auxiliary_pk != NULL; m++)
	{

		if(auxiliary_pk->element_scope == PRIMARY_KEY)
		{
			strcpy(primary_key, auxiliary_pk ->element_name);
		}

		if((auxiliary_pk->next_element == NULL) || (auxiliary_pk->next_element->element_scope == 0))
		{
			break;
		}

		auxiliary_pk = auxiliary_pk->next_element;
	}

	return primary_key;
}

int is_pk(element_instance *list_pointer, char primary_key[MAX])
{

	element_instance *auxiliary_pkey;
	auxiliary_pkey = list_pointer;
	int found = 0;
	int n = 0;
	for (n = 0; auxiliary_pkey != NULL; n++)
	{
		if(auxiliary_pkey->element_scope == COLUMN)
		{
			int validate_pk = search_column(primary_key, auxiliary_pkey);
			if(validate_pk == FOUND)
			{
				found = 1;
			}
		}

		if((auxiliary_pkey->next_element == NULL) || (auxiliary_pkey->next_element->element_scope == 0))
		{
			break;
		}

	 	auxiliary_pkey = auxiliary_pkey->next_element;
	 }

	 return found;
}

#endif