/* Author Mohib Ahmed
Email:mohhib.ahmed79@myhunter.cuny.edu
in this section we are implemnting the team file
still running into issue of diffrentiating bags when there is a nil bid and a non nil bid on the same team*/

#include "Team.h"
Team::Team(const PlayerComponents& player1, const PlayerComponents& player2)
{
    //Team(const Team& rhs);
	//Team(Team&& rhs) noexcept;za
    team.push_back(Player_factory(player1.name, player1.strategy));
    team.push_back(Player_factory(player2.name, player2.strategy));

    total_bags = 0;
    game_score = 0;
}
//@param player which player to call the function on
std::string Team::get_name(int player) const
{
    if (valid_player(player))
        return team[player]->get_name();
    else
        throw std::invalid_argument("Invalid player value"); 
}

//@param player which player to call the function on
void Team::set_bid(int player)
{
    if (valid_player(player))
        team[player]->set_bid();
    else
        throw std::invalid_argument("Invalid player value"); 
}

//@param player which player to call the function on
bool Team::add_trick(int player)
{
    if (valid_player(player))
        return team[player]->add_trick();
    else
        throw std::invalid_argument("Invalid player value"); 
}
//@param player which player to call the function on 

bool Team::add_card(int player, const Card& card)
{
    if (valid_player(player))
        return team[player]->add_card(card);
    else
        throw std::invalid_argument("Invalid player value"); 
}

//* @param card Card to add to the hand of player
Card Team::lead_card(int player, bool broken_spade)
{
    if (valid_player(player))
        return team[player]->lead_card(broken_spade);
    else
        throw std::invalid_argument("Invalid player value"); 
}
Card Team::play_card(int player, Suit led_suit)
{
    if (valid_player(player))
        return team[player]->play_card(led_suit);
    else
        throw std::invalid_argument("Invalid player value"); 
}
////Call reset on all the players of the team
void Team::reset()
{
    for (Player* player : team)
        player->reset();
}

/*Calculates the score of the team and returns the `Score` object. Must calculate the values in this order:
1. Nil Score
2. Bid Score
3. Total Bags
4. Hand Score
5. Game Score*/
Score Team::get_score()
{
    Score score;
    int tricks = 0;
    int non_nil_tricks = 0;
    int bid = 0;
    int possible_bags = 0;

    for (int player = 0; player < 2; ++player)
    {
        tricks += team[player]->get_trick();
        non_nil_tricks += (team[player]->nil_bid() ? 0 : team[player]->get_trick());
        bid += (team[player]->nil_bid() ? 0 : team[player]->get_bid());
        possible_bags += (team[player]->blind_nil_bid() ? 0 : team[player]->get_bid());
    }
    score.bid = bid;
    score.tricks = tricks;
    score.bags = possible_bags;
    score.bid_score = calculate_bid_score(non_nil_tricks, bid, score.bags, possible_bags);
    score.nil_score = calculate_nil_score(tricks, non_nil_tricks, bid, possible_bags);
    score.hand_score = calculate_hand_score(score.bid_score, score.nil_score, score.bag_penalty);
    score.total_bags = update_total_bags(score.bags, score.bag_penalty);
	score.game_score = update_game_score(score.hand_score);

    return score;
}


//Destructor for `Team`. Should deallocate the players in the vector.
Team::~Team()
{
    for (Player* player : team)
        delete player;
}
//Should print out the team in the format: `Team <player_one_name>/<player_two_name>` \(there is no newline at the end\).
std::ostream& operator<<(std::ostream& os, const Team& team)
{
    os << "Team " << team.team[0]->get_name() << '/' << team.team[1]->get_name();
    return os;
}
//Return true if player value is $0$ or $1$.
bool Team::valid_player(int player) const
{
    return (player >= 0 && player < 2);
}
//Calculate the nil score of the team
// Modifies the tricks, bid, and possible bags 
int Team::calculate_nil_score(int& tricks, int& non_nil_tricks, int& bid, int& possible_bags)
{
    int nil_score = 0;

    for (Player* player : team)
    {
        int player_tricks = player->get_trick();
        tricks += player_tricks;
        non_nil_tricks += player->nil_bid() ? 0 : player_tricks;
        bid += player->get_bid();
        
        
        if (player->nil_bid() || player->blind_nil_bid())
        {
            if (player_tricks > 0)
                possible_bags = 0;

            if (player->blind_nil_bid()) {
                if (player_tricks > 0) {
                    nil_score -= 200; // -200 if failed to get 0 tricks
                } else {
                    nil_score += 200; // + 200 if succeded to get 0 tricks
                }
            } else {
                if (player_tricks > 0) {
                    nil_score -= 100; // - 100 if pfailed to get 0 tricks
                } else {
                    nil_score += 100; // +100 if succeded to get 0 tricks
                }
            }
        }
    }

    return nil_score;
}
/*Changed the function prototype so that `non_nil_tricks` is the parameter rather than `tricks`
- If both players bid any type of nil, then just return 0
- If the number of `non_nil_tricks` is less than the `bid`, then there are no `bags` and `bid_score` is the negation of ten times `bid`
- Else, the number of `bags` is the addition of the `possible_bags`, `non_nil_tricks`, and negation of `bid`; the `bid_score` is `bid` times ten plus `bags`*/
int Team::calculate_bid_score(int non_nil_tricks, int bid, int& bags, int possible_bags)
{
    int bid_score = 0;

    if (non_nil_tricks >= bid & bid !=0)//we need bid !=0 due to avoid this executing for nil bids
    {
        bid_score = bid * 10 + (non_nil_tricks - bid);
        bags = non_nil_tricks - bid;
    }
    else
    {
        bid_score = -(bid * 10);
        bags = 0;
    }

    return bid_score;
}
// Returns the total bags
int Team::update_total_bags(int bags, int& bag_penalty)
{
    total_bags += bags;
    int updated_bags = total_bags;
	 if (updated_bags >=10)
    { 
		updated_bags = updated_bags-10; 
		bag_penalty = -100;//peanlty of 100
    }
    else
    {
        bag_penalty = 0;
    }

    return updated_bags;
}
//Calculate the `hand_score` and return it. The `hand_score` is just the summation of the `bid_score`, `nil_score`, and `bag_penalty`.
int Team::calculate_hand_score(int bid_score, int nil_score, int bag_penalty)
{
    return bid_score + nil_score + bag_penalty;
}
//Update the `game_score` attribute of the `Team` class and return it. The `game_score` value is adding the `hand_score` onto the `game_score`.
int Team::update_game_score(int hand_score)
{
    game_score += hand_score;
    return game_score;
}
/*Should print out the score in the format below (include a new line at the end of the last line):
```
Bid: <value>
Tricks: <value>
Bags: <value>
Bid Score: <value>
Nil Score: <value>
Bag Penalty: <value>
Hand Score: <value>
Total Bags: <value>
Game Score: <value>
```*/
std::ostream& operator<<(std::ostream& os, const Score& score)
{
    os << "Bid: " << score.bid << std::endl;
    os << "Tricks: " << score.tricks << std::endl;
    os << "Bags: " << score.bags << std::endl;
    os << "Bid Score: " << score.bid_score << std::endl;
    os << "Nil Score: " << score.nil_score << std::endl;
    os << "Bag Penalty: " << score.bag_penalty << std::endl;
    os << "Hand Score: " << score.hand_score << std::endl;
    os << "Total Bags: " << score.total_bags << std::endl;
    os << "Game Score: " << score.game_score << std::endl;

    return os;
}
