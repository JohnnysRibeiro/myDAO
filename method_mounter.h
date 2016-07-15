#ifndef METHOD_MOUNTER
#define METHOD_MOUNTER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **write_array_type(int dimension, int i, char type_array[][MAX]);
void mount_method_insert(FILE *file_out, char name_array[][MAX],  char type_array[][MAX],int real_dimension, char primary_key[MAX]);
void mount_method_select(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX]);
void mount_method_select_all(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX]);
void mount_method_update(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX]);
void mount_method_delete(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX]);
void mount_method_specific_select(FILE *file_out, char name_array[][MAX], select_instance* select_list_pointer, char selected_fields_array[][MAX], char selected_fields_type_array[][MAX], int offset);
void capitalize_name(char capitalized_name[MAX]);	

char **write_array_type(int dimension, int i, char type_array[][MAX])
{
    char** type_out =(char**)malloc(dimension*sizeof(char*));
	char integer_type[MAX];
	char varchar_type[MAX];
	char float_type[MAX];
	char table_type[MAX];

	strcpy(integer_type, "INT");
	strcpy(varchar_type, "VARCHAR");
	strcpy(float_type, "FLOAT");
	strcpy(table_type, "TABLE");

	for(i = 0; i < dimension; i++)
	{
		if((strcmp(type_array[i], table_type)) == 0)
		{
			type_out[i] = "table";
		}
		if((strcmp(type_array[i], integer_type)) == 0)
		{
			type_out[i] = "Integer";
		}		
		if((strcmp(type_array[i], varchar_type)) == 0)
		{
			type_out[i] = "String";
		}
		if((strcmp(type_array[i], float_type)) == 0)
		{
			type_out[i] = "float";
		}
	}
	return type_out;
}

void capitalize_name(char capitalized_name[MAX])
{
	capitalized_name[0] = toupper(capitalized_name[0]);
}

void uncapitalize_name(char uncapitalized_name[MAX])
{
	uncapitalized_name[0] = tolower(uncapitalized_name[0]);
}

void mount_method_insert(FILE *file_out, char name_array[][MAX],  char type_array[][MAX], int real_dimension, char primary_key[MAX])
{	
	// Capitalizing names
	char capital_entity_name[MAX];
	strcpy(capital_entity_name, name_array[0]);
	capitalize_name(capital_entity_name);

	char lowercase_entity_name[MAX];
	strcpy(lowercase_entity_name, name_array[0]);
	uncapitalize_name(lowercase_entity_name);

	char capital_primary_key[MAX];
	strcpy(capital_primary_key, primary_key);
	capitalize_name(capital_primary_key);

	int k = 0;
	char **type_out;
	type_out = write_array_type(real_dimension, k, type_array);

	//Escrevendo carcaça do método INSERT
	fprintf(file_out, "	public void insert(%s %s) {\n", capital_entity_name, lowercase_entity_name);
	fprintf(file_out, "\t\tSQLiteDatabase database = this.getWritableDatabase();\n\n");
	fprintf(file_out, "\t\tString sql = 'INSERT INTO %s (", name_array[0]);

	// fprintf(file_out, "\t\t\tString sql = 'INSERT INTO %s (s, s) VALUE (?, ?)';\n", name_array[0]);

	int i = 0; 
	for (i = 1; i < real_dimension; ++i)
	{
		if (i != (real_dimension - 1))
		{
			fprintf(file_out, "%s, ", name_array[i]);	
		}
		else
		{
			fprintf(file_out, "%s)' +\n", name_array[i]);	
		}
		
	}

	fprintf(file_out, "\t\t\t' VALUES ( ' +\n");

	int j = 0;
	for (j = 1; j < real_dimension; ++j)
	{
		char capital_column_name[MAX];
		strcpy(capital_column_name, name_array[j]);
		capitalize_name(capital_column_name);
	
		if(j != (real_dimension - 1))
		{
			if(strcmp(type_out[j], "String") == 0)
			{
				fprintf(file_out, "\t\t\t%s.get%s() + ', ' +\n", lowercase_entity_name, capital_column_name);	
			}
			else
			{
				fprintf(file_out, "\t\t\t%s.get%s().toString() + ', ' +\n", lowercase_entity_name, capital_column_name);	
			}
		}
		else
		{
			if(strcmp(type_out[j], "String") == 0)
			{
				fprintf(file_out, "\t\t\t%s.get%s() + ')';\n", lowercase_entity_name, capital_column_name);	
			}
			else
			{
				fprintf(file_out, "\t\t\t%s.get%s().toString() + ')';\n", lowercase_entity_name, capital_column_name);	
			}		
		}
		
	}

	fprintf(file_out, "\n\t\tdatabase.execSQL(sql);\n");

	fprintf(file_out, "\t}");

}

void mount_method_update(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX])
{	
	char entity_name_pascalcase[MAX];
	strcpy(entity_name_pascalcase, name_array[0]);
	capitalize_name(entity_name_pascalcase);

	char lowercase_entity_name[MAX];
	strcpy(lowercase_entity_name, name_array[0]);
	uncapitalize_name(lowercase_entity_name);

	char capital_primary_key[MAX];
	strcpy(capital_primary_key, primary_key);
	capitalize_name(capital_primary_key);
	int i = 0;

	int k = 0;
	char **type_out;
	type_out = write_array_type(real_dimension, k, type_array);


	// Refatorar método de buscar primary key
	char type_primary_key[MAX];
	for(k = 0; k < real_dimension; k++){
		if(strcmp(name_array[k],primary_key) == 0){
			strcpy(type_primary_key, type_out[k]);
		}
	}

	char capital_type_primary_key[MAX];
	strcpy(capital_type_primary_key, type_primary_key);
	capitalize_name(capital_type_primary_key);

	fprintf(file_out, "	public void update(%s %s) {\n", entity_name_pascalcase, lowercase_entity_name);
	fprintf(file_out, "\t\t\tSQLiteDatabase database = this.getWritableDatabase();\n\n");
	fprintf(file_out, "\t\t\tString sql = 'UPDATE %s SET' +\n", name_array[0]);
	
	for(i = 1; i<real_dimension; i++)
	{
		char capital_column_name[MAX];
		strcpy(capital_column_name, name_array[i]);
		capitalize_name(capital_column_name);
	
		if(strcmp(name_array[i],primary_key) != 0 && i != (real_dimension - 1))
		{
			if(strcmp(type_out[i], "String") == 0)
			{
				fprintf(file_out, "\t\t\t'%s = ' + %s.get%s() + ', ' +\n", name_array[i], lowercase_entity_name, capital_column_name);	
			}
			else	
			{
				fprintf(file_out, "\t\t\t'%s = ' + %s.get%s().toString() + ', ' +\n", name_array[i], lowercase_entity_name, capital_column_name);	
			}
		} 
		else if (strcmp(name_array[i],primary_key) != 0)
		{
			if(strcmp(type_out[i], "String") == 0)
			{
				fprintf(file_out, "\t\t\t'%s = ' + %s.get%s() +\n", name_array[i], lowercase_entity_name, capital_column_name);	
			}
			else	
			{
				fprintf(file_out, "\t\t\t'%s = ' + %s.get%s().toString() +\n", name_array[i], lowercase_entity_name, capital_column_name);	
			}
		}
	}
	
	fprintf(file_out, "\t\t\t' WHERE %s =' + %s.get%s();\n\n", primary_key, lowercase_entity_name, capital_primary_key);
	fprintf(file_out, "\t\t\tdatabase.execSQL(sql);\n");
	fprintf(file_out, "\t\t}");
		
}

void mount_method_delete(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX])
{	
	char entity_name_pascalcase[MAX];
	strcpy(entity_name_pascalcase, name_array[0]);
	capitalize_name(entity_name_pascalcase);

	char lowercase_entity_name[MAX];
	strcpy(lowercase_entity_name, name_array[0]);
	uncapitalize_name(lowercase_entity_name);

	char capital_primary_key[MAX];
	strcpy(capital_primary_key, primary_key);
	capitalize_name(capital_primary_key);

	int k = 0;
	char **type_out;
	type_out = write_array_type(real_dimension, k, type_array);

	char type_primary_key[MAX];

	for(k = 0; k < real_dimension; k++){
		if(strcmp(name_array[k],primary_key) == 0){
			strcpy(type_primary_key, type_out[k]);
		}
	}

	char capital_type_primary_key[MAX];
	strcpy(capital_type_primary_key, type_primary_key);
	capitalize_name(capital_type_primary_key);

	fprintf(file_out, "	public void delete(%s %s) throws SQLException {\n", entity_name_pascalcase, lowercase_entity_name);
	fprintf(file_out, "\t\t\tString sql = 'DELETE FROM %s WHERE %s=?';\n", name_array[0], primary_key);
	fprintf(file_out, "\t\t\tPreparedStatement statement = conn.preparedStatement(sql);\n");
	
	char capital_column_name[MAX];
	strcpy(capital_column_name, primary_key);
	capitalize_name(capital_column_name);
	fprintf(file_out, "\t\t\tstatement.set%s(1, %s.get%s());\n", capital_type_primary_key, lowercase_entity_name, capital_column_name);
	
	fprintf(file_out, "\t\t\tint rowsInserted = statement.executeUpdate();\n");
	fprintf(file_out, "\t\t\tif (rowsInserted > 0) {\n");
	fprintf(file_out, "\t\t\t\tSystem.out.println('An existing %s was deleted successfully!');\n", lowercase_entity_name);
	fprintf(file_out, "\t\t\t}\n");
	fprintf(file_out, "\t\t}");
		
}

void mount_method_select(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX])
{	
	char entity_name_pascalcase[MAX];
	strcpy(entity_name_pascalcase, name_array[0]);
	capitalize_name(entity_name_pascalcase);

	char lowercase_entity_name[MAX];
	strcpy(lowercase_entity_name, name_array[0]);
	uncapitalize_name(lowercase_entity_name);

	char capital_primary_key[MAX];
	strcpy(capital_primary_key, primary_key);
	capitalize_name(capital_primary_key);

	int k = 0;
	char **type_out;
	type_out = write_array_type(real_dimension, k, type_array);

	char type_primary_key[MAX];

	for(k = 0; k < real_dimension; k++){
		if(strcmp(name_array[k],primary_key) == 0){
			strcpy(type_primary_key, type_out[k]);
		}
	}

	char capital_type_primary_key[MAX];
	strcpy(capital_type_primary_key, type_primary_key);
	capitalize_name(capital_type_primary_key);
	//Escrevendo carcaça do método INSERT
	fprintf(file_out, "	public %s select(%s %s) throws SQLException {\n",entity_name_pascalcase, type_primary_key, primary_key);
	fprintf(file_out, "\t\tString sql = 'SELECT * FROM %s WHERE %s = ?';\n", entity_name_pascalcase, primary_key);
	fprintf(file_out, "\t\t%s %s = new %s();\n", entity_name_pascalcase, lowercase_entity_name, entity_name_pascalcase);
	fprintf(file_out, "\t\tPreparedStatement statement = conn.preparedStatement(sql);\n");
	fprintf(file_out, "\t\tstatement.set%s(1, %s);\n", capital_type_primary_key, primary_key);
	fprintf(file_out, "\t\tResultSet result = statement.executeQuery(sql);\n");
	//Vai ser um for
	fprintf(file_out, "\t\twhile (result.next()) {\n");
	int i = 0,j = 1;
	for(i = 1; i<real_dimension; i++)
	{
		char capital_column_name[MAX];
		strcpy(capital_column_name, name_array[i]);
		capitalize_name(capital_column_name);

		char capital_type_column[MAX];
		strcpy(capital_type_column, type_out[i]);
		capitalize_name(capital_type_column);

		fprintf(file_out, "\t\t\t%s.set%s(result.get%s(%d));\n", lowercase_entity_name, capital_column_name, capital_type_column, i);
		j++;
	}
	fprintf(file_out, "\t\t}\n");
	fprintf(file_out, "\t\treturn %s;\n", lowercase_entity_name);
	
	fprintf(file_out, "\t}");
		
}

void mount_method_select_all(FILE *file_out, char name_array[][MAX], char type_array[][MAX], int real_dimension, char primary_key[MAX])
{	
	char entity_name_pascalcase[MAX];
	strcpy(entity_name_pascalcase, name_array[0]);
	capitalize_name(entity_name_pascalcase);

	char lowercase_entity_name[MAX];
	strcpy(lowercase_entity_name, name_array[0]);
	uncapitalize_name(lowercase_entity_name);

	char capital_primary_key[MAX];
	strcpy(capital_primary_key, primary_key);
	capitalize_name(capital_primary_key);

	int k = 0;
	char **type_out;
	type_out = write_array_type(real_dimension, k, type_array);

	char type_primary_key[MAX];

	for(k = 0; k < real_dimension; k++){
		if(strcmp(name_array[k],primary_key) == 0){
			strcpy(type_primary_key, type_out[k]);
		}
	}

	char capital_type_primary_key[MAX];
	strcpy(capital_type_primary_key, type_primary_key);
	capitalize_name(capital_type_primary_key);
	//Escrevendo carcaça do método INSERT
	fprintf(file_out, "	public ArrayList<%s> selectAll() throws SQLException {\n",entity_name_pascalcase);
	fprintf(file_out, "\t\tString sql = 'SELECT * FROM %s';\n", entity_name_pascalcase);
	fprintf(file_out, "\t\tArrayList<%s> %ss = new ArrayList<>();\n", entity_name_pascalcase, lowercase_entity_name);
	fprintf(file_out, "\t\tPreparedStatement statement = conn.preparedStatement(sql);\n");
	fprintf(file_out, "\t\tResultSet result = statement.executeQuery(sql);\n");

	fprintf(file_out, "\t\twhile (result.next()) {\n");
	fprintf(file_out, "\t\t\t%s %s = new %s();\n", entity_name_pascalcase, lowercase_entity_name, entity_name_pascalcase);

	int i = 0,j = 1;
	for(i = 1; i<real_dimension; i++)
	{
		char capital_column_name[MAX];
		strcpy(capital_column_name, name_array[i]);
		capitalize_name(capital_column_name);

		char capital_type_column[MAX];
		strcpy(capital_type_column, type_out[i]);
		capitalize_name(capital_type_column);

		fprintf(file_out, "\t\t\t%s.set%s(result.get%s(%d));\n", lowercase_entity_name, capital_column_name, capital_type_column, i);
		j++;
	}
	fprintf(file_out, "\t\t\tif (%s != null) {\n", lowercase_entity_name);
	fprintf(file_out, "\t\t\t\t%ss.add(%s);\n", lowercase_entity_name, lowercase_entity_name);
	fprintf(file_out, "\t\t\t}\n");
	fprintf(file_out, "\t\t}\n");
	fprintf(file_out, "\t\treturn %ss;\n", lowercase_entity_name);
	
	fprintf(file_out, "\t}");
		
}

void mount_method_specific_select(FILE *file_out, char name_array[][MAX], select_instance* select_list_pointer_write, char selected_fields_array[][MAX], char selected_fields_type_array[][MAX], int offset)
{

	char entity_name_pascalcase[MAX];
	strcpy(entity_name_pascalcase, name_array[0]);
	capitalize_name(entity_name_pascalcase);

	char lowercase_entity_name[MAX];
	strcpy(lowercase_entity_name, name_array[0]);
	uncapitalize_name(lowercase_entity_name);

	int k = 0;
	char **type_out;
	type_out = write_array_type(select_list_pointer_write->selection_fields, k, selected_fields_type_array);

	fprintf(file_out, "	public ArrayList<%s> select",entity_name_pascalcase);
	int z;
	for(z = offset; z < offset + select_list_pointer_write->selection_fields; z++)
	{
		char capital_column[MAX];
		strcpy(capital_column, selected_fields_array[z]);
		capitalize_name(capital_column);

		fprintf(file_out, "%s", capital_column);
	}
	fprintf(file_out, "All() throws SQLException {\n");
	fprintf(file_out, "\t\tString sql = 'SELECT ");
	for(z = offset; z < offset + select_list_pointer_write->selection_fields; z++)
	{
		fprintf(file_out, "%s", selected_fields_array[z]);
		if(z != (offset + select_list_pointer_write->selection_fields - 1))
			fprintf(file_out, ",");
	}
	fprintf(file_out, " FROM %s';\n", entity_name_pascalcase);
	fprintf(file_out, "\t\tArrayList<%s> %ss = new ArrayList<>();\n", entity_name_pascalcase, lowercase_entity_name);
	fprintf(file_out, "\t\tPreparedStatement statement = conn.preparedStatement(sql);\n");
	fprintf(file_out, "\t\tResultSet result = statement.executeQuery(sql);\n"); 

	fprintf(file_out, "\t\twhile (result.next()) {\n");
	fprintf(file_out, "\t\t\t%s %s = new %s();\n", entity_name_pascalcase, lowercase_entity_name, entity_name_pascalcase);

	int i = 0,j = 1;
	for(i = 0; i < select_list_pointer_write->selection_fields; i++)
	{
		char capital_column_name[MAX];
		strcpy(capital_column_name, selected_fields_array[i]);
		capitalize_name(capital_column_name);

		char capital_type_column[MAX];
		strcpy(capital_type_column, type_out[i]);
		capitalize_name(capital_type_column);

		fprintf(file_out, "\t\t\t%s.set%s(result.get%s(%d));\n", lowercase_entity_name, capital_column_name, capital_type_column, i);
		j++;
	}
	fprintf(file_out, "\t\t\tif (%s != null) {\n", lowercase_entity_name);
	fprintf(file_out, "\t\t\t\t%ss.add(%s);\n", lowercase_entity_name, lowercase_entity_name);
	fprintf(file_out, "\t\t\t}\n");
	fprintf(file_out, "\t\t}\n");
	fprintf(file_out, "\t\treturn %ss;\n", lowercase_entity_name);
	
	fprintf(file_out, "\t}");

}

#endif