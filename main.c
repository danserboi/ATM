/*SERBOI Florea-Dan - Grupa 315CB*/
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int main()
{
	TSubCList LC = NULL;
	TCardList card = NULL;
	int no_cards = 0, max_no, no_par, max_pos = -1;
	char buf_op[60], par[10][30];
	FILE *input = fopen("input.in", "r"), *output = fopen("output.out", "w");
	fscanf(input, "%d\n", &max_no);
	/*se aloca LC cu un numar maxim de subliste(carduri)*/
	LC = alloc_list(max_no);
	/*se citesc operatiile din fisier, linie cu linie*/
	while (fgets(buf_op, 60, input))
	{
		/*se extrag parametri operatiei*/
		extract_parameters(buf_op, par, & no_par);
		if (strcmp(par[0], "add_card") == 0)
		{
			if (search_card(LC, max_no, par[1]))
				fprintf(output, "The card already exists\n");
			else
			{
				no_cards++;
				/*numarul de carduri depaseste numarul maxim de carduri,
				se redimensioneaza baza*/
				if (no_cards > max_no)
					LC = redimensioning_list(&LC, &max_no, &max_pos);
				card = alloc_card(par[1], par[2], par[3], par[4]);
				add_card(sublist_from_position(LC, card_digits_sum(card) 
						 % max_no), card);
				/*se calculeaza pozitia maxima(ultima sublista nevida)*/
				if (card_digits_sum(card) % max_no > max_pos)
					max_pos = card_digits_sum(card) % max_no;
			}
		}
		if (strcmp(par[0], "show") == 0)
		{
			/*se afiseaza toata lista*/
			if (strcmp(par[1], "") == 0)
				show_list(LC, max_pos, output);
			/*altfel se afiseaza un card din lista*/
			else if (search_card(LC, max_no, par[1]))
				show_card(search_card(LC, max_no, par[1]), output);
		}
		if (strcmp(par[0], "delete_card") == 0)
		{
			delete_card(LC, max_no, par[1]);
			no_cards--;
		}
		if (strcmp(par[0], "insert_card") == 0)
			insert_card(LC, par[1], par[2], max_no, output);
		if (strcmp(par[0], "cancel") == 0)
			cancel(LC, par[1], max_no);
		if (strcmp(par[0], "unblock_card") == 0)
			unblock_card(LC, par[1], max_no);
		if (strcmp(par[0], "pin_change") == 0)
			pin_change(LC, par[1], par[2], max_no, output);
		if (strcmp(par[0], "balance_inquiry") == 0)
			balance_inquiry(LC, par[1], max_no, output);
		if (strcmp(par[0], "recharge") == 0)
			recharge(LC, par[1], atoi(par[2]) , max_no, output);
		if (strcmp(par[0], "cash_withdrawal") == 0)
			cash_withdrawal(LC, par[1], atoi(par[2]) , max_no, output);
		if (strcmp(par[0], "transfer_funds") == 0)
			transfer_funds(LC, par[1], par[2], atoi(par[3]) , max_no, output);
		if (strcmp(par[0], "reverse_transaction") == 0)
			reverse_transaction(LC, par[1], par[2], atoi(par[3]) , max_no, 
								output);
	}
	fclose(input);
	fclose(output);
	dealloc_list(&LC);
	return 0;
}
