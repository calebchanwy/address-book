

#include "include/address_book.h"

#include <stdexcept>
#include <algorithm>
#include <functional>
#include <set>
#include <unordered_map>

// Method that adds Entry person to a given map
void addToMap(char first_char, std::unordered_map<char, std::vector<AddressBook::Entry>> &map, const AddressBook::Entry &person)
{
	// Check if the first character in map.
	auto it = map.find(first_char);
	if (it != map.end())
	{
		// A vector of entries already exists for the first character.
		// Add current person to vector
		it->second.emplace_back(person);
	} 
	else
	{
		// A vector of entries does not exist for the first character.
		// Create new vector of entries, starting with current entry and add to map
		map.emplace(first_char,std::vector<AddressBook::Entry>{person});
	}
}


// Method that removes an Entry person to a given map
void removeFromMap(char first_char, std::unordered_map<char, std::vector<AddressBook::Entry>> & map, const AddressBook::Entry& person)
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
	// Check if person in lookup maps already.
	std::vector<AddressBook::Entry> query_first_name = find(person.first_name);
	std::vector<AddressBook::Entry> query_last_name = find(person.last_name);


	if (std::find(query_first_name.begin(),query_first_name.end(),person) != query_first_name.end() && 
		std::find(query_last_name.begin(),query_last_name.end(),person) != query_last_name.end()
	)
	{
		// If an identical person is in lookup maps, do not add the person.
		// Do nothing.
	}
	else
	{
		// Add person to lookup maps.
		addToMap(person.first_name[0],first_name_lookup_map,person);
		addToMap(person.last_name[0],last_name_lookup_map,person);
	}
}



// Method to remove a person entry from the address book.
void AddressBook::remove(AddressBook::Entry person)
{
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
std::vector<AddressBook::Entry> sortEntriesByField(std::unordered_map<char, std::vector<AddressBook::Entry>>& entries_map, std::function<bool(const AddressBook::Entry&, const AddressBook::Entry&)> comparator)
{
    std::vector<AddressBook::Entry> sorted_entries;

	// For each entry pair in the map, insert entries to sroted entries
    for (const auto &entry_pair : entries_map)
    {
        auto &char_entries = entry_pair.second;
        sorted_entries.insert(sorted_entries.end(), char_entries.begin(), char_entries.end());
    }

	// Using given comparator, sort the vector.
	std::sort(sorted_entries.begin(),sorted_entries.end(),comparator);
    return sorted_entries;
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
	for (const char &c : str)
	{
		result += std::toupper(c);
	}
	return result;
}

// Helper function to find matching entries in a lookup map and insert into a given set.
void findAndInsertEntries(const char &first_char, const std::unordered_map<char, std::vector<AddressBook::Entry>> &entries_map, const std::string &value, std::set<AddressBook::Entry> &query_entries)
{	
	// Sanitise given value to all upper case.
	std::string sanitised_value = toUpper(value);

	// Check if character in map
	auto it = entries_map.find(toupper(first_char));
	if (it!=entries_map.end())
	{
		// If character is present in map, get existing entries.
		std::vector<AddressBook::Entry> entries_matching_char = it->second;
		for (const auto &entry : entries_matching_char)
		{
			if (toUpper(entry.first_name).find(sanitised_value) != std::string::npos || toUpper(entry.last_name).find(sanitised_value) != std::string::npos)
			{
				// If entry partially matches or completely matches in the first name/last name, add to query.
				query_entries.insert(entry);
			}
		}
	}
}

// Function that returns a vector of entries which match or partially match the given name.
std::vector<AddressBook::Entry> AddressBook::find(const std::string &name)
{
	// Initialise vector to store resulting matching entries.
	std::set<AddressBook::Entry> query_entries;

	// For each lookup map, find insert matching entries to query_entries set.
	findAndInsertEntries(name[0],first_name_lookup_map,name,query_entries);
	findAndInsertEntries(name[0],last_name_lookup_map,name,query_entries);

	return {query_entries.begin(),query_entries.end()};
}
