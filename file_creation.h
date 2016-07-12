#ifndef FILE_CREATE
#define FILE_CREATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "method_mounter.h"

// Declaração das funcões relacionadas a geração de código
char *write_file_name(char name_array[][MAX], char type);
int is_pk(element_instance *list_pointer, char primary_key[MAX]);
char *getPK(element_instance *list_pointer); 
char **write_array_type(int dimension, int i, char type_array[][MAX]);
void write_java_file(element_instance *list_pointer, int dimension, char entity_name_validate[MAX]);
void write_java_DAO_file(element_instance *list_pointer, int dimension, char entity_name_validate[MAX], select_instance *select_list_pointer, selected_fields_instance *selected_fields_list_pointer);


char *write_file_name(char name_array[][MAX], char type)
{
	static char file_out_name[MAX], source_string[MAX];
	strcpy(file_out_name, name_array[0]);

	capitalize_name(file_out_name);

	if(type == 'm'){
		strcpy(source_string, ".java");
	}else{
		strcpy(source_string, "DAO.java");
	}
	strcat(file_out_name, source_string);

	return file_out_name;
}

int search_entity(char entity_name_insert[MAX])
{
	if(entity_list_pointer == NULL)
    {
        return NOT_FOUND;
    }
    else
    {
        entity_instance *auxiliary_pointer = entity_list_pointer;
        while(auxiliary_pointer != NULL)
        {
            if (strcmp(entity_name_insert, auxiliary_pointer->entity_name) == 0)
            {
                return FOUND;
            }
            auxiliary_pointer = auxiliary_pointer->next_entity;
        }
        return NOT_FOUND;
    }
}

int search_column(char entity_name_insert[MAX], element_instance *element_pointer)
{
	if(element_list_pointer == NULL)
    {
        return NOT_FOUND;
    }
    else
    {
        if ((strcmp(entity_name_insert, element_pointer->entity_name) == 0)
        	|| (strcmp(entity_name_insert, element_pointer->element_name) == 0))
        {
            return FOUND;
        }
        return NOT_FOUND;
    }
}

void write_java_file(element_instance *list_pointer, int dimension, char entity_name_validate[MAX])
{
	FILE *file_out;
	char name_array[dimension][MAX];
	char name_upcase[dimension][MAX];
	char type_array[dimension][MAX];
	int i, real_dimension = 0;

	if(list_pointer == NULL)
	{
		printf("There is no instance.\n");
		exit(1);
	}

	char *primary_key;
	primary_key = getPK(list_pointer);


	int found = is_pk(list_pointer, primary_key);

	 // Imprime mensagens de erro caso PK não exista na tabela
	 if(found == 0){
	 	printf("\nThis field: %s, does not exist! Please check your sql file.\n", primary_key);
		exit(1);
	 }

	element_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer;
	for(i = 0; auxiliary_pointer != NULL; i++)
	{
		// Validando se a coluna existe na entidade
		int validate_column = search_column(entity_name_validate, auxiliary_pointer);
		if(validate_column == FOUND)
		{
			if(auxiliary_pointer->element_scope != PRIMARY_KEY){

				strcpy(name_array[i], auxiliary_pointer->element_name);
				strcpy(type_array[i], auxiliary_pointer->element_type);
				real_dimension++;
			}
		}

		else{
			printf("ERROR! Element does not belong in entity %s.", entity_name_validate);
		}

		if(auxiliary_pointer->next_element == NULL)
		{
			break;
		}
		else if(auxiliary_pointer->next_element->element_scope == 0)
		{
			break;
		}

		auxiliary_pointer = auxiliary_pointer->next_element;
	}

	char **type_out;
	type_out = write_array_type(dimension, i, type_array);


	char *file_out_name; 
	file_out_name = write_file_name(name_array, 'm');

	char folder_name[MAX];
	strcpy(folder_name, "Model/");
	strcat(folder_name,file_out_name);

	char entity_name_pascalcase[MAX];
	strcpy(entity_name_pascalcase, name_array[0]);
	capitalize_name(entity_name_pascalcase);

	printf("Writing java file for entity %s...\n",entity_name_pascalcase);
	file_out = fopen(folder_name, "w");

	if(!file_out)
	{
		printf("I can't open java file called %s.\n", file_out_name);
		exit(1);
	}

	fprintf(file_out, "import java.io.Serializable;\n\n");

	fprintf(file_out, "public class %s implements Serializable{\n", entity_name_pascalcase);
	fprintf(file_out, "\n");

	for(i = 1; i < real_dimension; i ++)
	{
		fprintf(file_out, "	private %s %s;\n", type_out[i], name_array[i]);
	}

	fprintf(file_out, "\n");
	fprintf(file_out, "	public %s() {\n", entity_name_pascalcase);
	fprintf(file_out, "\n");
	fprintf(file_out, "	}\n\n");

	//Escrevendo Gettings
	for(i = 1; i < real_dimension; i ++)
	{	
		//Transformando primeiro char em maiusculo
		strcpy(name_upcase[i], name_array[i]);
		name_upcase[i][0] = toupper(name_array[i][0]);
		
		fprintf(file_out, "	public %s get%s () {\n", type_out[i], name_upcase[i]);
		fprintf(file_out, "\t\treturn this.%s;\n", name_array[i]);
		fprintf(file_out, "	}\n\n");
	}

	//Escrevendo Settings
	for(i = 1; i < real_dimension; i ++)
	{
		//Transformando primeiro char em maiusculo
		strcpy(name_upcase[i], name_array[i]);
		name_upcase[i][0] = toupper(name_array[i][0]);
		
		fprintf(file_out, "	public void set%s (%s %s) {\n", name_upcase[i], type_out[i], name_array[i]);
		fprintf(file_out, "\t\tthis.%s = %s;\n", name_array[i], name_array[i]);
		fprintf(file_out, "	}\n\n");		
	}

	//Fechando classe
	fprintf(file_out, "}");

	printf("Writing java file for entity %s completed.\n",entity_name_pascalcase);

	fclose(file_out);
}


void write_java_DAO_file(element_instance *list_pointer, int dimension, char entity_name_validate[MAX], select_instance *select_list_pointer, selected_fields_instance *selected_fields_list_pointer)
{
  	char name_array[dimension][MAX];
	char name_upcase[dimension][MAX];
	char type_array[dimension][MAX];
	char primary_key[MAX];
	int i, real_dimension = 0;

	if(list_pointer == NULL)
	{
		printf("There is no instance.\n");
		exit(1);
	}

	element_instance *auxiliary_pointer;
	auxiliary_pointer = list_pointer;
	for(i = 0; auxiliary_pointer != NULL; i++)
	{
		int validate_column = search_column(entity_name_validate, auxiliary_pointer);
		if(validate_column == FOUND)
		{
			if(auxiliary_pointer->element_scope == PRIMARY_KEY)
			{
				strcpy(primary_key, auxiliary_pointer->element_name);
			}
			else
			{
				strcpy(name_array[i], auxiliary_pointer->element_name);
				strcpy(type_array[i], auxiliary_pointer->element_type);
				real_dimension++;
			}

		}
		else
		{
			printf("ERROR! Element does not belong in entity %s.", entity_name_validate);
		}	

		if(auxiliary_pointer->next_element == NULL)
		{
			break;
		}
		else if(auxiliary_pointer->next_element->element_scope == 0)
		{
			break;
		}

		auxiliary_pointer = auxiliary_pointer->next_element;
	}

	int number_specific_selects = 0;
	int number_of_selected_fields = 0;
	select_instance *auxiliary_select_pointer;
	auxiliary_select_pointer = select_list_pointer->next_select;

	while(auxiliary_select_pointer != NULL)
	{
		if(strcmp(auxiliary_select_pointer->entity_name, name_array[0]) == 0)
		{
			number_specific_selects++;
			number_of_selected_fields = number_of_selected_fields + auxiliary_select_pointer->selection_fields;
		}
		auxiliary_select_pointer = auxiliary_select_pointer->next_select;
	}

	selected_fields_instance *auxiliary_selected_list;
	auxiliary_selected_list = selected_fields_list_pointer->next_selected_field;

	char selected_fields_array[number_of_selected_fields][MAX];

	int counter = 0;
	while(auxiliary_selected_list != NULL)
	{
		if(strcmp(auxiliary_selected_list->select->entity_name, name_array[0]) == 0)
		{
			strcpy(selected_fields_array[counter], auxiliary_selected_list->field_name);
			counter++;
		}
		auxiliary_selected_list = auxiliary_selected_list->next_selected_field;
	}

	char selected_fields_type_array[number_of_selected_fields][MAX];

	int type_counter;

	for(type_counter = 0; type_counter < number_of_selected_fields; type_counter++)
	{
		auxiliary_pointer = list_pointer;
		while(auxiliary_pointer != NULL)
	{
		if((strcmp(auxiliary_pointer->element_name, selected_fields_array[type_counter]) == 0) &&
			auxiliary_pointer->element_scope == COLUMN)
		{
			strcpy(selected_fields_type_array[type_counter], auxiliary_pointer->element_type);
		}

		if(auxiliary_pointer->next_element == NULL)
		{
			break;
		}
		else if(auxiliary_pointer->next_element->element_scope == 0)
		{
			break;
		}

		auxiliary_pointer = auxiliary_pointer->next_element;
		}
	}

	FILE *file_out;
	char *file_out_name; 
  
	file_out_name = write_file_name(name_array, 'd');

	char folder_name[MAX];
	strcpy(folder_name, "DAO/");
	strcat(folder_name,file_out_name);

	char entity_name_pascalcase[MAX];
	strcpy(entity_name_pascalcase, name_array[0]);
	capitalize_name(entity_name_pascalcase);

	printf("Writing java %sDAO file...\n", entity_name_pascalcase);
	file_out = fopen(folder_name, "w");

	if(!file_out)
	{
		printf("I can't open javaDAO file.\n");
		exit(1);
	}

	// Escrita dos imports
	fprintf(file_out, "import android.content.Context;\n");
	fprintf(file_out, "import android.database.Cursor;\n");
	fprintf(file_out, "import android.database.sqlite.SQLiteDatabase;\n");
	fprintf(file_out, "import android.database.sqlite.SQLiteOpenHelper;\n\n");

	fprintf(file_out, "public class %sDAO extends SQLiteOpenHelper {\n", entity_name_pascalcase);
	fprintf(file_out, "\n");

	fprintf(file_out, "\tpublic %sDAO(Context context, String databaseName, int databaseVersion) {\n", entity_name_pascalcase);
	fprintf(file_out, "\t\tsuper(context, databaseName, null, databaseVersion);\n");
	fprintf(file_out, "\t}\n\n");

	fprintf(file_out, "\tpublic onCreate(SQLiteDatabase database) {\n", entity_name_pascalcase);
	fprintf(file_out, "\t}\n\n");

	fprintf(file_out, "\tpublic onUpgrade(SQLiteDatabase database, int oldVersion, int newVersion) {\n", entity_name_pascalcase);
	fprintf(file_out, "\t\tdatabase.execSQL('DROP TABLE IF EXISTS %s');\n", name_array[0]);
	fprintf(file_out, "\t\tonCreate(database);\n");
	fprintf(file_out, "\t}\n\n");

	mount_method_insert(file_out, name_array, type_array, real_dimension, primary_key);
	fprintf(file_out, "\n\n");

	mount_method_select(file_out, name_array, type_array, real_dimension, primary_key);
	fprintf(file_out, "\n\n");

	mount_method_select_all(file_out, name_array, type_array, real_dimension, primary_key);
	fprintf(file_out, "\n\n");

	mount_method_update(file_out, name_array, type_array, real_dimension, primary_key);
	fprintf(file_out, "\n\n");

	mount_method_delete(file_out, name_array, type_array, real_dimension, primary_key);
	fprintf(file_out, "\n\n");
	select_instance *auxiliary_select_pointer_write;
	auxiliary_select_pointer_write = select_list_pointer->next_select;

	int offset = 0;

	while(auxiliary_select_pointer_write != NULL)
	{
		if(strcmp(auxiliary_select_pointer_write->entity_name, name_array[0]) == 0)
		{
			mount_method_specific_select(file_out, name_array, auxiliary_select_pointer_write, selected_fields_array, selected_fields_type_array, offset);
			fprintf(file_out, "\n\n");
			offset = offset + auxiliary_select_pointer_write->selection_fields;
		}
		auxiliary_select_pointer_write = auxiliary_select_pointer_write->next_select;
	}

	fprintf(file_out, "\n}");

	printf("Writing java %sDAO file completed.\n",entity_name_pascalcase);

	free(auxiliary_select_pointer);
	free(auxiliary_selected_list);
}

#endif