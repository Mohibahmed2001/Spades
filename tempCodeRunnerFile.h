#pragma once

#include <iostream>
#include <string>

// ---------------------------------Rank---------------------------------------
// Represents the rank of a playing card. Since this is an enum, the ranks are 
// represented with integers starting from zero to twelve. However, not only is
// it an enum but rather an enum class, so referring to the values of the rank
// requires you to use the scope resolution operator. Using enum classes makes 
// references to the values more strict.
//		Example: Rank r1 = Rank::FIVE;
//				 Rank r2 = Rank::JACK;
// Enums and enum classes allow you to compare the types values since they are
// just integers under the hood.
//		Example: r1 <= r2 // True
// Since we've overloaded the input and output operators below, we can use Rank
// objects with streams.
//		Example: cout << r1; // prints "Five"
//				 cin >> r2;	 // reads a string from cin and updates r2
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
enum class Rank
{
	TWO, THREE, FOUR, FIVE, SIX, SEVEN, 
	EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};

/* Converts a string representing a Rank to a Rank object
* @pre The str represents a valid Rank ("Two", "Three", ... , "Ace")
*	If the string is not valid,
	throw std::invalid_argument("Invalid value for string_to_rank function.");
* @param str The string representing the rank
* @return Rank object
*/
Rank string_to_rank(const std::string& str);

/* Prints the rank to the stream object
* @param os The ostream object to print to
* @param rank The rank to print 
* @return reference to ostream object passed into the function
*/
std::ostream& operator<<(std::ostream& os, Rank rank);

/* Updates the rank object to the rank from the stream
* @pre The read input must be a valid rank, (HINT: use string_to_rank)
* @param is The istream object where the data is coming from
* @param rank The rank to update
* @return reference to istream object passed into the function
*/
std::istream& operator>>(std::istream& is, Rank& rank);





// ---------------------------------Suit---------------------------------------
// Suit is analogous to Rank in that it represents the suit of the card.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
enum class Suit
{
	CLUBS, DIAMONDS, HEARTS, SPADES
};


/* Converts a string representing a Suit to a Suit object
* @pre The str represents Suit ("Spades", "Hearts", "Clubs", or "Diamonds")
*	If the string is not valid,
*	throw std::invalid_argument("Invalid value for string_to_suit function.");
* @param str The string representing the suit
* @return Suit object
*/
Suit string_to_suit(const std::string& str);


/* Prints the suit to the stream object
* @param os The ostream object to print to
* @param suit The suit to print
* @return reference to ostream object passed into the function
*/
std::ostream& operator<<(std::ostream& os, Suit suit);


/* Updates the suit object to the suit from the stream
* @pre The read input must be a valid suit, (HINT: use string_to_suit)
* @param is The istream object where the data is coming from
* @param suit The suit to update
* @return reference to istream object passed into the function
*/
std::istream& operator>>(std::istream& is, Suit& suit);