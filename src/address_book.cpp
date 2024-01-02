

#include "include/address_book.h"

#include <stdexcept>
#include <algorithm>
#include <functional>
#include <set>
#include <map>

// Method that adds Entry person to a given map
void addToMap(char first_char, std::map<char, std::set<AddressBook::Entry>> &map, const AddressBook::Entry &person)
{
	// Check if the first character in map.

	auto it = map.find(toupper(first_char));
	if (it != map.end())
	{
		// A set of entries already exists for the first character.
		// Add current person to set.
		std::set<AddressBook::Entry> &entries = map[toupper(first_char)];
		entries.insert(person);
	} 
	else
	{
		// A set of entries does not exist for the first character.
		// Create new set of entries, starting with current entry and add to map
		map.emplace(first_char,std::set<AddressBook::Entry>{person});
	}
}


// Method that removes an Entry person to a given map
void removeFromMap(char first_char, std::map<char, std::set<AddressBook::Entry>> & map, const AddressBook::Entry& person)
{
	// Check if the first character in map.
	auto it = map.find(first_char);
	if (it != map.end())
	{
		// Remove person from existing set
		std::set<AddressBook::Entry> &entries_at_char = it->second;
		entries_at_char.erase(person);
	} 
	else
	{
		// A set of entries does not exist for the first character.
		// Do nothing
	}
}

// Method to add a person entry into the address book.
void AddressBook::add(AddressBook::Entry person)
{
	// Add person to lookup maps.
	addToMap(person.first_name[0],first_name_lookup_map,person);
	addToMap(person.last_name[0],last_name_lookup_map,person);
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
std::vector<AddressBook::Entry> sortEntriesByField(std::map<char, std::set<AddressBook::Entry>>& entries_map, std::function<bool(const AddressBook::Entry&, const AddressBook::Entry&)> comparator)
{
    std::vector<AddressBook::Entry> sorted_entries;

	// For each entry pair in the map, insert entries to sorted entries
	// As a map and set is already sorted, we only need to combine all sets together.
    for (auto &entry_pair : entries_map)
    {
        auto &char_entries = entry_pair.second;
        sorted_entries.insert(sorted_entries.end(), char_entries.begin(), char_entries.end());
    }

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
    std::string result(str);
	
	// Use transform lambda function to apply toupper to each char.
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) { return std::toupper(c); });
    return result;
}


// Helper function to find matching entries in a lookup map and insert into a given set.
void findAndInsertEntries(const char &first_char, const std::map<char, std::set<AddressBook::Entry>> &entries_map, const std::string &value, std::set<AddressBook::Entry> &query_entries)
{	
	// Sanitise given value to all upper case.
	std::string sanitised_value = toUpper(value);

	// Check if character in map
	try
	{
		// Character is present in the map.
		std::set<AddressBook::Entry> entries_matching_char = entries_map.at(toupper(first_char));
		for (const auto &entry : entries_matching_char)
		{
			// Check for first name match
			if (toUpper(entry.first_name).find(sanitised_value) != std::string::npos)
			{
				// If entry partially matches or completely matches in the first name, add to query.
				query_entries.emplace(entry);
			}

			// Check for last name match
			if (toUpper(entry.last_name).find(sanitised_value) != std::string::npos)
			{
				// If entry partially matches or completely matches in the last name, add to query.
				query_entries.emplace(entry);
			}
		}
	}
	catch(const std::out_of_range& e)
	{
		// Character not in map, do nothing
	}
	
}

// Function that returns a vector of entries which match or partially match the given name.
std::vector<AddressBook::Entry> AddressBook::find(const std::string &name)
{
	// Initialise set to store resulting matching entries, which removes duplicates.
	std::set<AddressBook::Entry> query_entries;

	// For each lookup map, find insert matching entries to query_entries set.
	findAndInsertEntries(name[0],first_name_lookup_map,name,query_entries);
	findAndInsertEntries(name[0],last_name_lookup_map,name,query_entries);

	// Return as a vector.
	return {query_entries.begin(),query_entries.end()};
}
