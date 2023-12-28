

#include "include/address_book.h"

#include <stdexcept>
#include <algorithm>
#include <functional>
#include <set>

// Method that adds Entry person to a given map
void addToMap(char first_char, std::map<char, std::vector<AddressBook::Entry>> &map, const AddressBook::Entry &person)
{
	// Check if the first character in map.
	auto it = map.find(first_char);
	if (it != map.end())
	{
		// A vector of entries already exists for the first character.
		// Add current person to vector
		it->second.push_back(person);
	} 
	else
	{
		// A vector of entries does not exist for the first character.
		// Create new vector of entries, starting with current entry
		std::vector<AddressBook::Entry> entries = {person};
		map.insert({first_char,entries});
	}
}


// Method that removes an Entry person to a given map
void removeFromMap(char first_char, std::map<char, std::vector<AddressBook::Entry>> & map, const AddressBook::Entry& person)
{
	// Check if the first character in map.
	auto it = map.find(first_char);
	if (it != map.end())
	{
		// Remove person from existing vector
		std::vector<AddressBook::Entry> &entries_at_char = it->second;
		auto it_person = std::find(entries_at_char.begin(),entries_at_char.end(),person);
		if (it_person!=entries_at_char.end())
		{
			// Remove person from vector
			entries_at_char.erase(it_person);
		}
		else{
			// Person not found at vector for this character
			// Do nothing
		}
	} 
	else
	{
		// A vector of entries does not exist for the first character.
		// Do nothing
	}
}


// Method to add a person entry into the address book.
void AddressBook::add(AddressBook::Entry person)
{
	// Add person to entries vector.
	entries.push_back(person);

	// Add person to lookup maps
	addToMap(person.first_name[0],first_name_lookup_map,person);
	addToMap(person.last_name[0],last_name_lookup_map,person);
}



// Method to remove a person entry from the address book.
void AddressBook::remove(AddressBook::Entry person)
{
	// Check if person is in address book
	auto it = std::find(entries.begin(), entries.end(), person);

	if (it!=entries.end())
	{
		// Entry in address book, safely remove.
		entries.erase(it);
	}
	else
	{
		// Entry not in address book, do nothing.
	}

	// Remove from lookup maps
	removeFromMap(person.first_name[0],first_name_lookup_map,person);
	removeFromMap(person.last_name[0],last_name_lookup_map,person);
}

// Comparison function between two entries' first names.
bool compareEntriesFirstName(const AddressBook::Entry &entry1, const AddressBook::Entry &entry2)
{
	return entry1.first_name < entry2.first_name;
}

// Comparison function between two entries' last names.
bool compareEntriesLastName(const AddressBook::Entry &entry1, const AddressBook::Entry &entry2)
{
	return entry1.last_name < entry2.last_name;
}

// Function that sorts a given map of entries by a given field, returning the final sorted vector.
std::vector<AddressBook::Entry> sortEntriesByField(std::map<char, std::vector<AddressBook::Entry>>& entriesMap, std::function<bool(const AddressBook::Entry&, const AddressBook::Entry&)> comparator)
{
    std::vector<AddressBook::Entry> sortedEntries;
    for (auto& entryPair : entriesMap)
    {
        auto& charEntries = entryPair.second;
        std::sort(charEntries.begin(), charEntries.end(), comparator);
        sortedEntries.insert(sortedEntries.end(), charEntries.begin(), charEntries.end());
    }
    return sortedEntries;
}

// Function that returns a vector of entries, sorted by the first name.
std::vector<AddressBook::Entry> AddressBook::sortedByFirstName()
{	
	return sortEntriesByField(first_name_lookup_map,compareEntriesFirstName);
}

// Function that returns a vector of entries, sorted by the last name.
std::vector<AddressBook::Entry> AddressBook::sortedByLastName()
{
	return sortEntriesByField(last_name_lookup_map,compareEntriesLastName);
}

// Function that converts a given string into all upper case.
std::string toUpper(const std::string &str)
{
	std::string result;
	for (char c : str)
	{
		result += std::toupper(c);
	}
	return result;
}

std::vector<AddressBook::Entry> findEntryInMap(char first_char, std::map<char, std::vector<AddressBook::Entry>> &map, const std::string &value)
{
	// Initialise vector to store resulting matching entries.
	std::vector<AddressBook::Entry> query_entries;

	// Check if character in map
	auto it = map.find(first_char);
	if (it!=map.end())
	{
		std::vector<AddressBook::Entry> &entries_matching_char = it->second;
		for (auto &entry : entries_matching_char)
		{
			if (toUpper(entry.first_name).find(value) != std::string::npos || toUpper(entry.last_name).find(value) != std::string::npos || entry.phone_number.find(value)!= std::string::npos )
			{
				query_entries.push_back(entry);
			}
		}
		return query_entries;
	}
	else
	{
		// Character not in map, return empty query.
		return query_entries;
	}
}

// Function that returns a vector of entries which match or partially match the given name.
std::vector<AddressBook::Entry> AddressBook::find(const std::string &name)
{
	// Initialise vector to store resulting matching entries.
	std::vector<AddressBook::Entry> query_entries;

	// Sanitise given name to all upper case.
	std::string sanitised_name = toUpper(name);

	std::vector<AddressBook::Entry> first_name_matching_first_char= findEntryInMap(sanitised_name[0],first_name_lookup_map,sanitised_name);
	std::vector<AddressBook::Entry> last_name_matching_first_char= findEntryInMap(sanitised_name[0],last_name_lookup_map,sanitised_name);
	query_entries.insert(query_entries.end(),first_name_matching_first_char.begin(),first_name_matching_first_char.end());
	query_entries.insert(query_entries.end(),last_name_matching_first_char.begin(),last_name_matching_first_char.end());
	
	return query_entries;
}
