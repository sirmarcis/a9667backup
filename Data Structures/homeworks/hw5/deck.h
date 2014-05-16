#include "playing_card.h"
#include "node.h"

// ==================================================================================
// prototypes for provided functions (implemented in main.cpp)
// ==================================================================================

bool SanityCheckPrimary(Node* top);
bool SanityCheckSorted(Node* first);
Node* CreateDeck(int numCards);
void DeckPushBackCard(Node* &deck, const std::string& suit, int num);
void PrintDeckPrimary(const std::string &description, Node* deck);

//added function prototypes
int DeckSize(Node *head);
void PrintDeckSorted(std::string prefix, Node *head);
bool SamePrimaryOrder(Node *head1, Node *head2);
bool ReversePrimaryOrder(Node *head1, Node *head2);
Node* CopyDeck(Node* head);
void DeleteAllCards(Node* &head);
void CutDeck(Node *head, Node* &top, Node* &bottom, std::string keyString);
Node* Shuffle(Node* &top, Node* &bottom, std::string keyString);
void Deal(Node* &head, Node** hands, int numHands, std::string numCardsToDeal, int numCardsPerHand);
Node *SortHand(Node* head);


// ==================================================================================
//
// Write all the prototypes for the your functions here
// (they will be implemented in deck.cpp)
//
// NOTE: These are standalone functions, not part of a class.
//
// You may include additional helper functions as necessary
// All of your code must be in deck.h and deck.cpp
//
// ==================================================================================


