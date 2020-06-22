/*SERBOI Florea-Dan - Grupa 315CB*/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

/* returneaza adresa istoricului creat sau NULL*/
THistoryList alloc_history(char *operations)
{
	/*incearca alocare, altfel returneaza NULL*/
	THistoryList aux = NULL;
	aux = (THistoryList)calloc(1, sizeof(THistory));
	if (aux)
	{
		/*aloca sirul operatiei*/
		aux->operations = calloc(strlen(operations) + 1, sizeof(char));
		/*daca nu reuseste, elibereaza ce a fost alocat anterior
		si returneaza NULL*/
		if (!aux->operations)
		{
			free(aux);
			return NULL;
		}
		/*completeaza istoricul*/
		strcpy(aux->operations, operations);
		aux->next_history = NULL;
	}
	return aux;
}

/*dealoca istoric, si adresa devine NULL*/
void dealloc_history(AHistoryList history)
{
	free((*history)->operations);
	free(*history);
	*history = NULL;
}

/* returneaza adresa cardului creat sau NULL*/
TCardList alloc_card(char *card_number, char *pin,
                     char *expiry_date, char *cvv)
{
	/*incearca alocare, altfel returneaza NULL*/
	TCardList aux = NULL;
	aux = (TCardList)calloc(1, sizeof(TCard));
	if (aux)
	{
		/*completeaza informatiile cardului*/
		strcpy(aux->card_number, card_number);
		strcpy(aux->pin, pin);
		strcpy(aux->expiry_date, expiry_date);
		strcpy(aux->cvv, cvv);
		aux->balance = 0;
		strcpy(aux->status, "NEW");
		aux->failures = 0;
		aux->history = NULL;
		aux->next_card = NULL;
	}
	return aux;
}

/*dealoca un card, si adresa devine NULL*/
void dealloc_card(ACardList card)
{
	THistoryList aux = NULL, history = (*card)->history;
	/*dealoca toate elementele de tip istoric*/
	while (history)
	{
		aux = history;
		history = history->next_history;
		dealloc_history(&aux);
	}
	free(*card);
	*card = NULL;
}

/* returneaza adresa sublistei create sau NULL*/
TSubCList alloc_sublist(int position, TCardList card)
{
	/*incearca alocare, altfel returneaza NULL*/
	TSubCList sub = NULL;
	sub = calloc(1, sizeof(TSubC));
	if (sub)
	{
		/*completeaza campurile*/
		sub->position = position;
		sub->sub_cards = card;
		sub->next_sub = NULL;
	}
	return sub;
}

/*dealoca o sublista, adresa devine NULL*/
void dealloc_sublist(ASubCList sub)
{
	TCardList aux = NULL, card = (*sub)->sub_cards;
	while (card)
	{
		/*dealoca fiecare card pe care lista il contine*/
		aux = card;
		card = card->next_card;
		dealloc_card(&aux);
	}
	free(*sub);
	*sub = NULL;
}

/*aloca o lista care contine un anumit numar de subliste*/
/*returneaza adresa sublistei create sau NULL*/
TSubCList alloc_list(int max_no)
{
	TSubCList head = NULL, ant = NULL, sub = NULL, aux = NULL;
	int i;
	for (i = 0; i < max_no; ant = sub, i++)
	{
		/*incearca sa aloce o sublista*/
		sub = alloc_sublist(i, NULL);
		/*daca nu reuseste, elibereaza tot ce a fost alocat pana atunci
		si returneaza NULL*/
		if (sub == NULL)
		{
			while (head)
			{
				aux = head;
				head = head->next_sub;
				dealloc_sublist(&aux);
			}
			fprintf(stderr, "Alocarea listei nu a reusit!");
			return NULL;
		}
		/*daca reuseste, creaza legaturile intre subliste*/
		if (i == 0)
			head = sub;
		else
			ant->next_sub = sub;
	}
	return head;
}

/*dealoca o lista, adresa devine NULL*/
void dealloc_list(ASubCList sub)
{
	TSubCList aux = NULL, aux_sub = *sub;
	while (aux_sub)
	{
		aux = aux_sub;
		aux_sub = aux_sub->next_sub;
		dealloc_sublist(&aux);
	}
	*sub = NULL;
}

/*adauga in istoricul unui card o operatie*/
void add_history(TCardList card, char *operations)
{
	/*incearca sa aloce o celula istoric*/
	THistoryList new_history = NULL;
	new_history = alloc_history(operations);
	/*daca reuseste alocarea, creaza legaturile*/
	if (new_history)
	{
		THistoryList aux = card->history;
		card->history = new_history;
		new_history->next_history = aux;
	}
	else
		fprintf(stderr, "Alocarea istoricului nu a reusit!");
}

/*adauga un card intr-o sublista*/
void add_card(TSubCList sub, TCardList card)
{
	/*se creaaza legaturile, cardul este adaugat la inceputul sublistei*/
	TCardList aux = sub->sub_cards;
	sub->sub_cards = card;
	sub->sub_cards->next_card = aux;
}

/*afiseaza tot istoricul unui card*/
void show_history(TCardList card, FILE *output)
{
	THistoryList history = card->history;
	fprintf(output, "history: [");
	while (history)
	{
		fprintf(output, "(%s)", history->operations);
		history = history->next_history;
		if (history)
			fprintf(output, ", ");
	}
	fprintf(output, "]");
}

/*afiseaza toate informatiile cardului*/
void show_card(TCardList card, FILE *output)
{
	fprintf(output, "(");
	fprintf(output, "card number: %s, ", card->card_number);
	fprintf(output, "PIN: %s, ", card->pin);
	fprintf(output, "expiry date: %s, ", card->expiry_date);
	fprintf(output, "CVV: %s, ", card->cvv);
	fprintf(output, "balance: %d, ", card->balance);
	fprintf(output, "status: %s, ", card->status);
	show_history(card, output);
	fprintf(output, ")\n");
}

/*afiseaza o sublista*/
void show_sublist(TSubCList sub, FILE *output)
{
	if (sub)
	{
		fprintf(output, "pos%d: [", sub->position);
		TCardList head = sub->sub_cards;
		while (head)
		{
			show_card(head, output);
			head = head->next_card;
		}
		fprintf(output, "]");
	}
}

/*afiseaza continutul unei liste
pana la ultima sublista nevida (pozitia maxima din lista) */
void show_list(TSubCList list, int max_no, FILE *output)
{
	int i;
	TSubCList sub = list;
	for (i = 0; i <= max_no; i++)
	{
		if (sub)
		{
			fprintf(output, "pos%d: [", sub->position);
			TCardList head = sub->sub_cards;
			if (head)
				fprintf(output, "\n");
			while (head)
			{
				show_card(head, output);
				head = head->next_card;
			}
			fprintf(output, "]\n");
		}
		sub = sub->next_sub;
	}
}

/*returneaza suma cifrelor numarului unui card*/
int card_digits_sum(TCardList card)
{
	int i, sum = 0;
	for (i = 0; i <= 15; i++)
		sum = sum + (int)card->card_number[i] - 48;
	return sum;
}

/*returneaza sublista de pe o anumita pozitie din cadrul listei*/
TSubCList sublist_from_position(TSubCList list, int pos)
{
	int i;
	TSubCList sub = list;
	for (i = 0; i < pos; i++)
		sub = sub->next_sub;
	return sub;
}

/*cauta un card in lista si il retuneaza*/
TCardList search_card(TSubCList list, int max_no, char *card_number)
{
	int i;
	TCardList aux = NULL;
	TSubCList sub = list;
	for (i = 0; i < max_no; i++)
	{
		aux = sub->sub_cards;
		while (aux)
		{
			if (strcmp(aux->card_number, card_number) == 0)
				return aux;
			aux = aux->next_card;
		}
		sub = sub->next_sub;
	}
	return NULL;
}

/*sterge un card dintr-o lista, returneaza 1/0 pentru succes/esec*/
int delete_card(TSubCList list, int max_no, char *card_number)
{
	int i;
	TCardList aux = NULL, ant = NULL;
	TSubCList sub = list;
	/*parcurgem fiecare sublista*/
	for (i = 0; i < max_no; i++)
	{
		ant = NULL;
		aux = sub->sub_cards;
		/*parcurgem fiecare card dintr-o sublista*/
		while (aux)
		{
			/*daca gasim cardul, rupem legaturile si dealocam cardul*/
			if (strcmp(aux->card_number, card_number) == 0)
			{
				if (ant == NULL)
					sub->sub_cards = sub->sub_cards->next_card;
				else
					ant->next_card = aux->next_card;
				dealloc_card(&aux);
				return 1;
			}
			/*altfel avansam in lista cardurilor*/
			else
			{
				ant = aux;
				aux = aux->next_card;
			}
		}
		sub = sub->next_sub;
	}
	return 0;
}

/*anuleaza o tranzactie, daca este posibil*/
void reverse_transaction(TSubCList list, char *card_n_source,
                         char *card_n_dest, int sum, int max_no,
                         FILE *output)
{
	char operation[70];
	/*cauta cardurile sursa si destinatie*/
	TCardList card_source = search_card(list, max_no, card_n_source);
	TCardList card_dest = search_card(list, max_no, card_n_dest);
	THistoryList ant_history = NULL,
	             history_source = NULL,
	             history_dest = NULL;
	/*daca sunt suficienti bani pe contul destinatarului pentru a anula*/
	if (card_dest->balance >= sum)
	{
		/*in operation, retinem operatia transferului dintre conturi*/
		sprintf(operation, "SUCCESS, transfer_funds %s %s %d",
		        card_n_source, card_n_dest, sum);
		history_source = card_source->history;
		history_dest = card_dest->history;
		/*vom sterge operatia transferului din istoricul destinatarului*/
		/*daca operatia se gaseste la inceputul istoricului,
		schimbam adresa de inceput si dealocam istoricul*/
		if (strcmp(history_dest->operations, operation) == 0)
		{
			card_dest->history = card_dest->history->next_history;
			dealloc_history(&history_dest);
		}
		/*daca se gaseste dupa inceput,
		rupem legaturile si dealocam istoricul*/
		else
		{
			while (strcmp(history_dest->operations, operation) != 0)
			{
				ant_history = history_dest;
				history_dest = history_dest->next_history;
			}
			ant_history->next_history = history_dest->next_history;
			dealloc_history(&history_dest);
		}
		/*in operation, retinem operatia de anulare a tranzactiei*/
		sprintf(operation, "SUCCESS, reverse_transaction %s %s %d",
		        card_n_source, card_n_dest, sum);
		/*incearca sa aloce o celula istoric*/
		history_source = alloc_history(operation);
		/*daca reuseste alocarea, creaza legaturile si
		modifica sumele de pe conturile implicate*/
		if (history_source)
		{
			history_source->next_history = card_source->history;
			card_source->history = history_source;
			card_source->balance += sum;
			card_dest->balance -= sum;
		}
		else
			fprintf(stderr, "Alocarea istoricului nu a reusit!");
	}
	else
		fprintf(output, "The transaction cannot be reversed\n");
}

/*deblocheaza un card, numarul de incercari se reseteaza la 0*/
void unblock_card(TSubCList list, char *card_number, int max_no)
{
	TCardList card = search_card(list, max_no, card_number);
	card->failures = 0;
	strcpy(card->status, "ACTIVE");
}

/*redimensioneaza o lista, se creaza o lista cu nr dublu de elemente
pe care o returneaza*/
TSubCList redimensioning_list(ASubCList list, int *max_no, int *max_pos)
{
	*max_no *= 2;
	/*se incearca alocarea unei noi liste, cu nr dublu de elemente*/
	TSubCList new_lista_carduri = NULL, sub = *list;
	new_lista_carduri = alloc_list(*max_no);
	if (!new_lista_carduri)
	{
		fprintf(stderr, "Alocarea noii liste nu a reusit!");
		return NULL;
	}
	TCardList aux, card = NULL;
	/*parcurgem sublista*/
	while (sub != NULL)
	{
		/*parcurgem fiecare card din sublista si il adaugam in noua lista,
		modificand si pozitia maxima a noii liste*/
		aux = sub->sub_cards;
		while (aux)
		{
			card = aux;
			aux = aux->next_card;
			card->next_card = NULL;
			add_card(sublist_from_position(new_lista_carduri,
			         card_digits_sum(card) % (*max_no)), card);
			if (card_digits_sum(card) % (*max_no) > *max_pos)
				*max_pos = card_digits_sum(card) % (*max_no);
		}
		/*ne asiguram ca sublista veche nu mai indica catre niciun card*/
		sub->sub_cards = NULL;
		/*trecem la o sublista noua*/
		sub = sub->next_sub;
	}
	/*dealocam lista veche*/
	dealloc_list(list);
	return new_lista_carduri;
}

/*insereaza un card in bancomat, daca nu reuseste afiseaza mesajele
caracteristice*/
void insert_card(TSubCList list, char *card_number, char *pin, int max_no,
                 FILE *output)
{
	char operation[70];
	/*cauta cardul in lista*/
	TCardList card = search_card(list, max_no, card_number);
	THistoryList history = NULL;
	/*daca pinul este introdus corect*/
	if (strcmp(card->pin, pin) == 0)
	{
		/*daca numarul de incercari nereusite consecutive e mai mic ca 3,
		il reseteaza la 0 si adauga operatia in istoric*/
		if (card->failures < 3)
		{
			card->failures = 0;
			if (strcmp(card->status, "NEW") == 0)
				fprintf(output, "You must change your PIN.\n");
			sprintf(operation, "SUCCESS, insert_card %s %s", card_number, pin);
			/*incearca sa aloce o celula istoric*/
			history = alloc_history(operation);
			/*daca reuseste, creaza legaturile*/
			if (history)
			{
				history->next_history = card->history;
				card->history = history;
			}
			else
				fprintf(stderr, "Alocarea istoricului nu a reusit!");
		}
		else
		{
			fprintf(output,
				"The card is blocked. Please contact the administrator.\n");
			sprintf(operation, "FAIL, insert_card %s %s", card_number, pin);
			history = alloc_history(operation);
			if (history)
			{
				history->next_history = card->history;
				card->history = history;
			}
			else
				fprintf(stderr, "Alocarea istoricului nu a reusit!");
		}
	}
	else
	{
		/*daca nu s-a ajuns la 3 incercari consecutive, se incrementeaza*/
		if (card->failures <= 2)
		{
			fprintf(output, "Invalid PIN\n");
			card->failures += 1;
		}
		if (card->failures == 3)
		{
			fprintf(output,
				"The card is blocked. Please contact the administrator.\n");
			strcpy(card->status, "LOCKED");
		}
		sprintf(operation, "FAIL, insert_card %s %s", card_number, pin);
		history = alloc_history(operation);
		if (history)
		{
			history->next_history = card->history;
			card->history = history;
		}
		else
			fprintf(stderr, "Alocarea istoricului nu a reusit!");
	}
}

/*se alimenteaza un card cu o anumita suma de bani, multiplu de 10*/
void recharge(TSubCList list, char *card_number, int sum, int max_no,
              FILE *output)
{
	char operation[70];
	TCardList card = search_card(list, max_no, card_number);
	if (sum % 10 == 0)
	{
		card->balance += sum;
		fprintf(output, "%d\n", card->balance);
		sprintf(operation, "SUCCESS, recharge %s %d", card_number, sum);
	}
	else
	{
		fprintf(output, "The added amount must be multiple of 10\n");
		sprintf(operation, "FAIL, recharge %s %d", card_number, sum);
	}
	THistoryList history = NULL;
	history = alloc_history(operation);
	if (history)
	{
		history->next_history = card->history;
		card->history = history;
	}
	else
		fprintf(stderr, "Alocarea istoricului nu a reusit!");
}

/*se scoate o suma de bani de pe card, multiplu de 10, daca e posibil*/
void cash_withdrawal(TSubCList list, char *card_number, int sum, int max_no, 
					 FILE *output)
{
	char operation[70];
	THistoryList history = NULL;
	TCardList card = search_card(list, max_no, card_number);
	if (sum % 10 == 0)
	{
		if (card->balance >= sum)
		{
			card->balance -= sum;
			fprintf(output, "%d\n", card->balance);
			sprintf(operation, "SUCCESS, cash_withdrawal %s %d",
					card_number, sum);
		}
		else
		{
			fprintf(output, "Insufficient funds\n");
			sprintf(operation, "FAIL, cash_withdrawal %s %d",
					card_number, sum);
		}
	}
	else
	{
		fprintf(output, "The requested amount must be multiple of 10\n");
		sprintf(operation, "FAIL, cash_withdrawal %s %d", card_number, sum);
	}
	history = alloc_history(operation);
	if (history)
	{
		history->next_history = card->history;
		card->history = history;
	}
	else
		fprintf(stderr, "Alocarea istoricului nu a reusit!");
}

/*se verifica soldul de pe card, se afiseaza acest sold*/
void balance_inquiry(TSubCList list, char *card_number, int max_no, 
					 FILE *output)
{
	char operation[70];
	TCardList card = search_card(list, max_no, card_number);
	fprintf(output, "%d\n", card->balance);
	sprintf(operation, "SUCCESS, balance_inquiry %s", card_number);
	THistoryList history = NULL;
	history = alloc_history(operation);
	if (history)
	{
		history->next_history = card->history;
		card->history = history;
	}
	else
		fprintf(stderr, "Alocarea istoricului nu a reusit!");
}

/*se transfera o suma de pe un card pe altul, multiplu de 10*/
void transfer_funds(TSubCList list, char *card_n_source, char *card_n_dest,
                    int sum, int max_no, FILE *output)
{
	char operation[70];
	TCardList card_source = search_card(list, max_no, card_n_source);
	TCardList card_dest = search_card(list, max_no, card_n_dest);
	THistoryList history_source = NULL, history_dest = NULL;
	if (sum % 10 == 0)
	{
		if (card_source->balance >= sum)
		{
			card_source->balance -= sum;
			card_dest->balance += sum;
			fprintf(output, "%d\n", card_source->balance);
			sprintf(operation, "SUCCESS, transfer_funds %s %s %d",
			        card_n_source, card_n_dest, sum);
			history_dest = alloc_history(operation);
			if (history_dest)
			{
				history_dest->next_history = card_dest->history;
				card_dest->history = history_dest;
			}
			else
				fprintf(stderr, "Alocarea istoricului nu a reusit!");
		}
		else
		{
			fprintf(output, "Insufficient funds\n");
			sprintf(operation, "FAIL, transfer_funds %s %s %d",
			        card_n_source, card_n_dest, sum);
		}
	}
	else
	{
		fprintf(output, "The transferred amount must be multiple of 10\n");
		sprintf(operation, "FAIL, transfer_funds %s %s %d",
		        card_n_source, card_n_dest, sum);
	}
	history_source = alloc_history(operation);
	if (history_source)
	{
		history_source->next_history = card_source->history;
		card_source->history = history_source;
	}
	else
		fprintf(stderr, "Alocarea istoricului nu a reusit!");
}

/*daca pinul e introdus corect, se schimba pinul iar statusul devine ACTIVE*/
void pin_change(TSubCList list, char *card_number, char *pin, int max_no,
                FILE *output)
{
	char operation[70];
	TCardList card = search_card(list, max_no, card_number);
	if (strlen(pin) == 4 && isdigit(pin[0]) && isdigit(pin[1])
	        && isdigit(pin[2]) && isdigit(pin[3]))
	{
		strcpy(card->pin, pin);
		strcpy(card->status, "ACTIVE");
		sprintf(operation, "SUCCESS, pin_change %s %s", card_number, pin);
	}
	else
	{
		fprintf(output, "Invalid PIN\n");
		sprintf(operation, "FAIL, pin_change %s %s", card_number, pin);
	}
	THistoryList history = NULL; 
	history = alloc_history(operation);
	if (history)
	{
		history->next_history = card->history;
		card->history = history;
	}
	else
		fprintf(stderr, "Alocarea istoricului nu a reusit!");
}

/*se scoate cardul din bancomat*/
void cancel(TSubCList list, char *card_number, int max_no)
{
	char operation[70];
	TCardList card = search_card(list, max_no, card_number);
	THistoryList history = NULL;
	sprintf(operation, "SUCCESS, cancel %s", card_number);
	history = alloc_history(operation);
	if(history)
	{
		history->next_history = card->history;
		card->history = history;
	}
	else
		fprintf(stderr, "Alocarea istoricului nu a reusit!");
}

/*se extrag parametri unei operatii*/
void extract_parameters(char operation[70], char parameters[10][30], int *n)
{
	char parameter[30];
	int k;
	unsigned int i;
	k = -1, *n = -1;
	for (i = 0; i <= 9; i++)
		strcpy(parameters[i], "");
	for (i = 0; i <= strlen(operation); i++)
		if (operation[i] != ' ' && operation[i] != '\0' 
			&& operation[i] != '\n')
		{
			k++;
			parameter[k] = operation[i];
		}
		else
		{
			parameter[k + 1] = '\0';
			if (strlen(parameter) > 0)
			{
				(*n)++;
				strcpy(parameters[*n], parameter);
			}
			k = -1;
			strcpy(parameter, "");
		}
	(*n)++;
}