

#include "include/address_book.h"

#include <stdexcept>
#include <algorithm>

// Method to add a person entry into the address book
void AddressBook::add(AddressBook::Entry person)
{
	entries.push_back(person);
}

// Method to remove a person entry from the address book
void AddressBook::remove(AddressBook::Entry person)
{
	// Check if person is in address book
	auto it = std::find(entries.begin(), entries.end(), person);

	if (it!=entries.end())
	{
		// Entry in address book, safely remove
		entries.erase(it);
	}
	else
	{
		// Entry not in address book, do nothing
	}
}

// Comparison function between two entries' first names
bool compareEntriesByFirstName(const AddressBook::Entry &entry1, const AddressBook::Entry &entry2)
{
	return entry1.first_name < entry2.first_name;
}

// Comparison function between two entries' last names
bool compareEntriesByLastName(const AddressBook::Entry &entry1, const AddressBook::Entry &entry2)
{
	return entry1.last_name < entry2.last_name;
}

// Function that returns a vector of entries, sorted by the first name
std::vector<AddressBook::Entry> AddressBook::sortedByFirstName()
{
	std::vector<Entry> sorted_entries = entries;
	std::sort(sorted_entries.begin(), sorted_entries.end(), compareEntriesByFirstName);
	return sorted_entries;
}

// Function that returns a vector of entries, sorted by the last name
std::vector<AddressBook::Entry> AddressBook::sortedByLastName()
{
	std::vector<Entry> sorted_entries = entries;
	std::sort(sorted_entries.begin(), sorted_entries.end(), compareEntriesByLastName);
	return sorted_entries;
}

// Function that converts a given string into all upper case
std::string toUpper(const std::string &str)
{
	std::string result;
	for (char c : str)
	{
		result += std::toupper(c);
	}
	return result;
}

// Function that returns a vector of entries which match or partially match the given name
std::vector<AddressBook::Entry> AddressBook::find(const std::string &name)
{
	// Initialise vector to store resulting matching entries
	std::vector<AddressBook::Entry> query_entries;

	// Sanitise given name to all upper case
	std::string sanitised_name = toUpper(name);

	for (auto &entry : entries)
	{
		// Sanatise first name and last name for comparison to be case insensitive
		std::string sanitised_first_name = toUpper(entry.first_name);
		std::string sanitised_last_name = toUpper(entry.last_name);

		if (sanitised_first_name.find(sanitised_name) != std::string::npos || sanitised_last_name.find(sanitised_name) != std::string::npos)
		{
			// If name query can be found as a substring in the first name or last name
			// Add to query entries
			query_entries.push_back(entry);
		}
	}
	return query_entries;
}
