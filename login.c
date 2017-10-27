#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Users {
  char username[20];
  char password[20];
};

struct Users users[2];

int validate(char username[20], char password[20]) {

  // Por enquanto vamos assumir que eixste 1 utilizador, depois adicionamos
  // mais
  for (int count = 0; count < 2; count++) {
    if (strcmp(username, users[count].username) == 0) {
      if (strcmp(password, users[count].password) == 0) {
        return 1;
      }
    }
  }
  return 0;
}

int main(int argc, char const *argv[]) {

  char username[20];
  char password[20];
  /* code */

  printf("Vamos inserir 2 utilizadores para testar\n\n");
  // Vamos pedir a inserção de 2 users só para testar, depois vamos buscar ao
  // ficheiro de texto
  for (int count = 0; count < 2; count++) {
    fflush(stdin);
    printf("\nUtilizador %d", count + 1);
    printf("\nLogin : ");
    scanf("%s", users[count].username);
    printf("\nPassword : ");
    scanf("%s", users[count].password);
  }

  printf("Vamos testar o login agora, insira as credênciais\n");
  printf("\nUsername : ");
  scanf("%s", username);
  printf("\nPassword : ");
  scanf("%s", password);

  if (validate(username, password) == 1) {
    printf("Login efetuado com sucesso\n");
  } else {
    printf("As credênciais estão erradas\n");
  }
  return 0;
}
