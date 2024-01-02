#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <set>

/// The main Address Book implementation. Extend as required.
class AddressBook
{
public:
	/// A container for address book data.
	struct Entry
	{
		std::string first_name;
		std::string last_name;
		std::string phone_number;

		// Add custom equality operator, comparing two entries.
		bool operator==(const Entry &other) const
		{
			// If all fields of the entry object are equal to each other, must be equal.
			return this->first_name == other.first_name && this->last_name == other.last_name && this->phone_number == other.phone_number;
		}

		// Add custom < operator, comparing two entries for sorting purposes
		bool operator<(const Entry &other) const
		{
			// One entry is < another by their first name and last name.
			return this->first_name < other.first_name || this->last_name < other.last_name;
		}

		// Overload << operator for string representation for debugging purposes.
		friend std::ostream& operator<<(std::ostream& os, const Entry& obj) {
			os << "Entry | ";
			os << "First Name: " << obj.first_name << ", ";
			os << "Last Name: " << obj.last_name << ", ";
			os << "Phone Number: " << obj.phone_number;
			return os;
		}
	};

	/// Add an entry. Implement in address_book.cpp.
	void add(Entry person);

	/// Remove an entry. Implement in address_book.cpp.
	void remove(Entry person);

	/// Return all entries sorted by first names. Implement in address_book.cpp.
	std::vector<Entry> sortedByFirstName();

	/// Return all entries sorted by last names. Implement in address_book.cpp.
	std::vector<Entry> sortedByLastName();

	/// Return all matching entries. Implement in address_book.cpp.
	std::vector<Entry> find(const std::string &name);

private:

	/**
	 * In order to aid the retrieval of entries for extremely large amounts of entries, I have
	 * implemented a lookup map, relating first character of person's last/first name to a list
	 * (vector) of entries.
	 * 
	 * This solution is efficient, when names are evenly spread accross characters. However,
	 * in reality it is often the case that certain characters are more popular, such as A, when
	 * compared to names beginning with X.
	 * 
	 * If I had more time, this can be refactored to use a indexing table, which would evenly
	 * split entries by their index, and easily retireved using a binary table. Moreover, I would
	 * look into refactoring the maps to improve scalability when adding more fields which can
	 * be searched, as adding a new lookup map requries changes in multiple functions.
	*/

	// Lookup map, grouping entries by first character of the first name.
	std::map<char, std::set<Entry>> first_name_lookup_map;

	// Lookup map, grouping entries by first character of the last name.
	std::map<char, std::set<Entry,std::less<Entry>>> last_name_lookup_map;

};