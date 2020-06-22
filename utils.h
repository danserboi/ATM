/*SERBOI Florea-Dan - Grupa 315CB*/

/*tipurile istoric, lista istoric si adresa lista istoric*/
typedef struct history
{
	char *operations;
	struct history *next_history;
} THistory, *THistoryList, **AHistoryList;

/*tipurile card, lista card si adresa lista card*/
typedef struct card
{
	char card_number[17];
	char pin[7];
	char expiry_date[6];
	char cvv[4];
	int balance;
	char status[8];
	int failures;
	THistoryList history;
	struct card *next_card;
} TCard, *TCardList, **ACardList;

/*tipurile sublista, lista sublista si adresa lista sublista*/
typedef struct sublist
{
	int position;
	TCardList sub_cards;
	struct sublist *next_sub;
} TSubC, *TSubCList, **ASubCList;

THistoryList alloc_history(char *operations);

void dealloc_history(AHistoryList history);

TCardList alloc_card(char *card_number, char *pin,
                     char *expiry_date, char *cvv);

void dealloc_card(ACardList card);

TSubCList alloc_sublist(int position, TCardList card);

void dealloc_sublist(ASubCList sub);

TSubCList alloc_list(int max_no);

void dealloc_list(ASubCList sub);

void add_history(TCardList card, char *operations);

void add_card(TSubCList sub, TCardList card);

void show_history(TCardList card, FILE *output);

void show_card(TCardList card, FILE *output);

void show_sublist(TSubCList sub, FILE *output);

void show_list(TSubCList list, int max_no, FILE *output);

int card_digits_sum(TCardList card);

TSubCList sublist_from_position(TSubCList list, int pos);

TCardList search_card(TSubCList list, int max_no, char *card_number);

int delete_card(TSubCList list, int max_no, char *card_number);

void reverse_transaction(TSubCList list, char *card_n_source,
                         char *card_n_dest, int sum, int max_no,
                         FILE *output);

void unblock_card(TSubCList list, char *card_number, int max_no);

TSubCList redimensioning_list(ASubCList list, int *max_no, int *max_pos);

void insert_card(TSubCList list, char *card_number, char *pin, int max_no,
                 FILE *output);

void recharge(TSubCList list, char *card_number, int sum, int max_no,
              FILE *output);

void cash_withdrawal(TSubCList list, char *card_number, int sum, int max_no, 
					 FILE *output);

void balance_inquiry(TSubCList list, char *card_number, int max_no, 
					 FILE *output);

void transfer_funds(TSubCList list, char *card_n_source, char *card_n_dest,
                    int sum, int max_no, FILE *output);

void pin_change(TSubCList list, char *card_number, char *pin, int max_no,
                FILE *output);

void cancel(TSubCList list, char *card_number, int max_no);

void extract_parameters(char operation[60], char parameters[10][30], int *n);