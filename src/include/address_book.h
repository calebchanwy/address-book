#pragma once

#include <string>
#include <vector>
#include <map>

/// The main Address Book implementation. Extend as required.
class AddressBook
{
public:
	/// A container for address book data
	struct Entry
	{
		std::string first_name;
		std::string last_name;
		std::string phone_number;

		// Add custom equality operator, comparing two entries
		bool operator==(const Entry &other) const
		{
			return this->first_name == other.first_name && this->last_name == other.last_name && this->phone_number == other.phone_number;
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
	// Vector containing all people entries in the address book
	std::vector<Entry> entries;
};