// ==================================================================================
// Implement all functions that you listed by prototype in deck.h
// You may include additional helper functions as necessary
// All of your code must be in deck.h and deck.cpp
//
// NOTE: These are standalone functions, not part of a class.
// ==================================================================================

#include <iostream>
#include <cassert>
#include "playing_card.h"
#include "node.h"
#include "deck.h"

void resetDeck(Node* &tail){
    while(tail->before != NULL)
        tail = tail->before;
}

int DeckSize(Node *head){
    int size_of = 0;
    Node *tmp = head;
    while(tmp != NULL){
        size_of++;
        tmp = tmp->after;
    }
    return size_of;
}

void PrintDeckSorted(std::string prefix, Node *head){
    Node *tmp = head;
    std::cout << prefix;
    while(tmp->sorted_prev != NULL){
        tmp = tmp->sorted_prev;
    }
    while(tmp != NULL){
        PlayingCard currCard = tmp->getCard();
        std::cout << " " << currCard.getString();
        tmp = tmp->sorted_next;
    }
    std::cout << std::endl;
}

bool SamePrimaryOrder(Node *head1, Node *head2){
    Node *tracer1 = head1;
    Node *tracer2 = head2;
    if(DeckSize(head1) == DeckSize(head2)){
        while(tracer1 != NULL && tracer2 != NULL){
            if(!(tracer1->getCard() == tracer2->getCard()))
                return false;
            tracer1 = tracer1->after;
            tracer2 = tracer2->after;
        }
        return true;
    }
    else
        return false;
}

bool ReversePrimaryOrder(Node *head1, Node *head2){
    Node *tracer1 = head1;
    Node *tracer2 = head2;
    while(tracer2->after != NULL){
        tracer2 = tracer2->after;
    }
    if(DeckSize(head1) == DeckSize(head2)){
        while(tracer1 != NULL && tracer2 != NULL){
            if(!(tracer1->getCard() == tracer2->getCard()))
                return false;
            tracer1 = tracer1->after;
            tracer2 = tracer2->before;
        }
        return true;
    }
    else
        return false;
}

Node* CopyDeck(Node *head){
    int newDeckSize = DeckSize(head);
    return CreateDeck(newDeckSize);
}

void DeleteAllCards(Node* &head){
    while(head->before != NULL){
        head = head->before;
    }
    while(head->after != NULL){
        Node* old = head;
        head = head->after;
        delete old;
    }
    //head = NULL;
    delete head;
}

Node* CreateDeckSegment(int count, int numCards) {
  Node *answer = NULL;
  int current_suit = 0;
  int current_face_value = 1;
  while (count < numCards) {
    DeckPushBackCard(answer,suit_names[current_suit],current_face_value);
    current_face_value++;
    if (current_face_value > 13) {
      current_face_value = 1;
      current_suit++;
      if (current_suit > 3) {
        current_suit = 0;
      }
    }
    count++;
  }
  return answer;
}

void CutDeck(Node *head, Node* &top, Node* &bottom, std::string keyString){

    if(keyString == "perfect"){
        int split_pnt = DeckSize(head)/2, cnts = 0;
        Node* topHead; Node* bottomHead;
        Node* prevTop = NULL; Node* prevBottom = NULL;
        Node* tmp = head;
        while(tmp != NULL){
            if(cnts < split_pnt ){
                if(top != NULL)
                    delete top;
                top = new Node(tmp->getCard());
                top->before = prevTop;
                if(prevTop != NULL)
                    prevTop->after = top;
                else
                    topHead = top;
                prevTop = top;
            }
            else if (cnts == split_pnt ){
                top->after = NULL;
                if(bottom != NULL)
                    delete bottom;
                bottom = new Node(tmp->getCard());
                bottom->before = prevBottom;
                if(prevBottom != NULL)
                    prevBottom->after = bottom;
                else
                    bottomHead = bottom;
                prevBottom = bottom;
            }
            else{
                delete bottom;
                bottom = new Node(tmp->getCard());
                bottom->before = prevBottom;
                if(prevBottom != NULL)
                    prevBottom->after = bottom;
                prevBottom = bottom;

            }
            tmp = tmp->after;
            cnts++;
        }
        top = topHead;
        bottom = bottomHead;
    }
}

Node* Shuffle(Node* &top, Node* &bottom, std::string keyString){
    //this is where my memory leaks
    if(keyString == "perfect"){
        //Node* shuffledDeck = NULL;
        Node* topTrace = top;
        Node* bottomTrace = bottom;
        Node* prevNode = NULL;
        while(topTrace != NULL && bottomTrace != NULL){
            if(prevNode != NULL)
                prevNode->after = topTrace;
            topTrace->before = prevNode;
            prevNode = topTrace;
            topTrace = topTrace->after;
            prevNode->after = bottomTrace;
            bottomTrace->before = prevNode;
            prevNode = bottomTrace;
            bottomTrace = bottomTrace->after;

            /*
            shuffledDeck = new Node(topTrace->getCard());
            shuffledDeck->before = prevNode;
            if(prevNode != NULL)
                prevNode->after = shuffledDeck;
            prevNode = shuffledDeck;
            shuffledDeck = new Node(bottomTrace->getCard());
            shuffledDeck->before = prevNode;
            prevNode->after = shuffledDeck;
            prevNode = shuffledDeck;
            topTrace = topTrace->after;
            bottomTrace = bottomTrace->after;
            */
        }
        topTrace = NULL; bottomTrace = NULL;
        resetDeck(top); resetDeck(bottom);
        bottom = NULL;
        std::cout << "   BOTTOM IS EQUAL TO: " << bottom << std::endl;
        //DeleteAllCards(top);
        //DeleteAllCards(bottom);
        //resetDeck(shuffledDeck);
        return top;
    }
    else
        return NULL;
}

void Deal(Node* &head, Node** hands, int numHands, std::string numCardsToDeal, int numCardsPerHand){
    //this works by looping through the deck and giving each hand a card untill either the deck is out or the hand has all the cards they need
    Node* dealer = head;
    Node* newHead = NULL;
    bool first_card = true;
    while(dealer != NULL){
        for(int x = 0; x < numHands; ++x){
            if(dealer != NULL){
                if(first_card){
                    delete hands[x];
                    hands[x] = new Node(dealer->getCard());
                    dealer = dealer->after;
                }
                else{
                    Node* card = hands[x];
                    int cnts = 0;
                    while(card->after != NULL){
                        card = card->after;
                        cnts++;
                    }
                    if(cnts < numCardsPerHand - 1){
                        Node* new_card = new Node(dealer->getCard());
                        card->after = new_card;
                        new_card->before = card;
                    }
                    else{
                        if(newHead == NULL){
                            delete newHead;
                            newHead = new Node(dealer->getCard());
                        }
                        else{
                            Node* new_card = new Node(dealer->getCard());
                            newHead->after = new_card;
                            new_card->before = newHead;
                            newHead = new_card;
                        }
                    }
                    resetDeck(hands[x]);
                    dealer = dealer->after;
                }
            }
        }
        first_card = false;
    }
    if(newHead != NULL)
        resetDeck(newHead);
    DeleteAllCards(head);
    head = newHead;
}

bool cardAlreadyPresent(Node* deck, Node* toCheck){
    //called by SortHand to check that the same card isn't being added twice
    Node* tmp = deck;
    while(tmp != NULL){
        if(tmp->getCard() == toCheck->getCard())
            return true;
        tmp = tmp->after;
    }
    return false;
}


void fetchHead(Node* &fakeHead){
    //called by SortHand to actually get to the tail of the list, not the head
    while(fakeHead->after != NULL)
        fakeHead = fakeHead->after;
}

void findSortedHead(Node* &head){
    //called by SortHand to get to the sorted begining of the deck
    while(head->sorted_prev != NULL){
        head = head->sorted_prev;
    }
}

Node *SortHand(Node* head){
    //loops through the deck and finds the smallest card in each case, creating a temporary deck to keep track of their order
    Node* newDeck = NULL;
    int oldDeckSize = DeckSize(head);
    int cnts = 0;
    Node* prevSmallestCard = NULL;
    for(int x = 0; x < oldDeckSize; x++){
        resetDeck(head);
        if(newDeck != NULL)
            resetDeck(newDeck);
        Node* currCard = head;
        Node* smallestCard = NULL;
        while(currCard != NULL){
            if(cardAlreadyPresent(newDeck, currCard) != true){
                if(smallestCard == NULL ){
                    smallestCard = currCard;
                }
                else if(currCard->getCard() < smallestCard->getCard()){
                    smallestCard = currCard;
                }
            }
            currCard = currCard->after;
        }
        //this was used in a privious attempt at sorting, it remains the same because it works to a degree in sorting
        if(smallestCard != NULL){
            if(cardAlreadyPresent(newDeck, smallestCard) != true){
                Node* newCard = new Node(smallestCard->getCard());
                if(newDeck != NULL){
                    fetchHead(newDeck);
                    newDeck->after = newCard;
                    newCard->before = newDeck;
                }
                smallestCard->sorted_prev = prevSmallestCard;
                if(prevSmallestCard != NULL)
                    prevSmallestCard->sorted_next = smallestCard;
                prevSmallestCard = smallestCard;
                newDeck = newCard;
                cnts++;
            }
        }
        else{
            Node* newCard = new Node(currCard->getCard());
            fetchHead(newDeck);
            newDeck->after = newCard;
            newCard->before = newDeck;
            cnts++;
            smallestCard->sorted_prev = prevSmallestCard;
            if(prevSmallestCard != NULL)
                prevSmallestCard->sorted_next = smallestCard;
            prevSmallestCard = smallestCard;
        }
    }
    resetDeck(newDeck);
    DeleteAllCards(newDeck);
    findSortedHead(head);
    return head;
}



//debugging helper console commands
//  cd  C:/Users/marava2/My\ Documents/Data\ Structures/homeworks/hw5
//   g++ playing_card.cpp deck.cpp main.cpp -o main.exe

//     i686-pc-mingw32-g++.exe -static-libgcc -static-libstdc++ -ggdb -o main.exe playing_card.cpp deck.cpp main.cpp
//     drmemory -brief -batch -- main.exe


// ==================================================================================
