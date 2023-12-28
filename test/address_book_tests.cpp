#include "address_book.h"

#include <gtest/gtest.h>
#include <string>

///  Sample test data
std::string people[][3] = {
		{"Sally", "Graham", "+44 7700 900297"},
		{"Phoenix", "Bond", "0161 496 0311"},
		{"Aaran", "Parks", ""},
		{"Jayden", "Riddle", "+44 131 496 0609"},
		{"Adriana", "Paul", "(739) 391-4868"},
		{"Hamza", "Bo", "+44 131 496 0571"}
	};

///  Sample test data
AddressBook AddTestPeople()
{
	AddressBook addressBook;
	// Add all of the test data to the address book
	for (auto person : people)
	{
		AddressBook::Entry entry = { person[0], person[1], person[2] };
		addressBook.add(entry);
	}
	return addressBook;
}


/// Tests that it is possible to add a person to the address book.
TEST(AddressBookTests, AddPerson)
{
	AddressBook ab;
	// Add just the first person to an empty address book
	AddressBook::Entry entry = { people[0][0], people[0][1], people[0][2] };
	ab.add(entry);

	// Get the contents of the address book
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();
	
	// There should only be 1 entry in the results
	ASSERT_EQ(results.size(), 1);

	// Validate that the name and phoen number is correct
	ASSERT_EQ(results[0].first_name, people[0][0]);
	ASSERT_EQ(results[0].last_name, people[0][1]);
	ASSERT_EQ(results[0].phone_number, people[0][2]);
}


/// Tests that entries are sorted by first name correctly.
TEST(AddressBookTests, SortedByFirstNames)
{
	// The correctly sorted test data 
	const std::string people_sortedFirstNames[][3] = {
		{"Aaran", "Parks", ""},
		{"Adriana", "Paul", "(739) 391-4868"},
		{"Hamza", "Bo", "+44 131 496 0571"},
		{"Jayden", "Riddle", "+44 131 496 0609"},
		{"Phoenix", "Bond", "0161 496 0311"},
		{"Sally", "Graham", "+44 7700 900297"},
	};

	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Sort by first names
	std::vector<AddressBook::Entry> results = ab.sortedByFirstName();

	// There should only be 6 entries in the results 
	ASSERT_EQ(results.size(), 6);

	// Validate that all of the results exactly match the desired output
	for (size_t i=0; i < results.size(); i++)
	{
		auto personResult = results[i];
		auto personAnswer = people_sortedFirstNames[i];
 
		ASSERT_EQ(personResult.first_name, personAnswer[0]);
		ASSERT_EQ(personResult.last_name, personAnswer[1]);
		ASSERT_EQ(personResult.phone_number, personAnswer[2]);
	}
}


/// Tests that entries are sorted by last name correctly.
TEST(AddressBookTests, SortedByLastNames)
{
	const std::string people_sortedLastNames[][3] = {
		{"Hamza", "Bo", "+44 131 496 0571"},
		{"Phoenix", "Bond", "0161 496 0311"},
		{"Sally", "Graham", "+44 7700 900297"},
		{"Aaran", "Parks", ""},
		{"Adriana", "Paul", "(739) 391-4868"},
		{"Jayden", "Riddle", "+44 131 496 0609"},
	};

	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Sort by last names
	std::vector<AddressBook::Entry> results = ab.sortedByLastName();

	// There should only be 6 entries in the results 
	ASSERT_EQ(results.size(), 6);

	// Validate that all of the results exactly match the desired output
	for (size_t i = 0; i < results.size(); i++)
	{
		auto personResult = results[i];
		auto personAnswer = people_sortedLastNames[i];

		ASSERT_EQ(personResult.first_name, personAnswer[0]);
		ASSERT_EQ(personResult.last_name, personAnswer[1]);
		ASSERT_EQ(personResult.phone_number, personAnswer[2]);
	}
}


/// Tests that an entry can be found in the address book.
TEST(AddressBookTests, FindPerson)
{
	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Find a person whose name is, or starts with "Graham"
	std::vector<AddressBook::Entry> results = ab.find("Graham");

	// There should only be exactly 1 entry in the results 
	ASSERT_EQ(results.size(), 1);

	// Validate that the result is the entry we expected
	ASSERT_EQ(results[0].first_name, "Sally");
	ASSERT_EQ(results[0].last_name, "Graham");
	ASSERT_EQ(results[0].phone_number, "+44 7700 900297");
}

/// Tests that an entry can be found in the address book.
TEST(AddressBookTests, FindMultiplePeople)
{
	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Add a person who has a first name Graham, but last name Johnson
	ab.add(AddressBook::Entry({"Graham","Johnson",""}));

	// Find a person whose name is, or starts with "Graham"
	std::vector<AddressBook::Entry> results = ab.find("Graham");

	// There should only be exactly 2 entries in the results 
	ASSERT_EQ(results.size(), 2);

	// Validate that the result is the entries we expect
	ASSERT_EQ(results[0].first_name, "Graham");
	ASSERT_EQ(results[0].last_name, "Johnson");
	ASSERT_EQ(results[0].phone_number, "+44 7700 900297");
	ASSERT_EQ(results[1].first_name, "Sally");
	ASSERT_EQ(results[1].last_name, "Graham");
	ASSERT_EQ(results[1].phone_number, "+44 7700 900297");
}

/// Tests that entries with a partial name match are retrieved correctly.
TEST(AddressBookTests, FindPersonalPartialMatch)
{
	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Find a person whose name is, or starts with "Graham"
	std::vector<AddressBook::Entry> results = ab.find("Grah");

	// There should only be exactly 1 entry in the results 
	ASSERT_EQ(results.size(), 1);

	// Validate that the result is the entry we expected
	ASSERT_EQ(results[0].first_name, "Sally");
	ASSERT_EQ(results[0].last_name, "Graham");
	ASSERT_EQ(results[0].phone_number, "+44 7700 900297");
}

/// Tests that retrieving entries are not case sensitive.
TEST(AddressBookTests, FindPersonCaseInsensitive)
{
	// Populate the address book
	AddressBook ab = AddTestPeople();

	// Find a person whose name is, or starts with "AARAN"
	std::vector<AddressBook::Entry> resultsUppercase = ab.find("AARAN");
	// Find a person whose name is, or starts with "aaran"
	std::vector<AddressBook::Entry> resultsLowercase = ab.find("aaran");

	// There should only be exactly 1 entry in both resuts
	ASSERT_EQ(resultsUppercase.size(), 1);
	ASSERT_EQ(resultsLowercase.size(), 1);

	// Validate that both the lowercase and uppercase results are the same entry
	ASSERT_EQ(resultsUppercase[0].first_name, resultsLowercase[0].first_name);
	ASSERT_EQ(resultsUppercase[0].last_name, resultsLowercase[0].last_name);
	ASSERT_EQ(resultsLowercase[0].phone_number, resultsLowercase[0].phone_number);
}


/// Tests that it is possible to remove a person from the address book
TEST(AddressBookTests, RemovePerson){

	// Populate the address book
	AddressBook ab = AddTestPeople();
	
	// Sort by last names
	std::vector<AddressBook::Entry> results = ab.sortedByLastName();

	// There should already be 6 existing entries in the results 
	ASSERT_EQ(results.size(), 6);

	// Save first two entries of results
	AddressBook::Entry person1 = results[0];
	AddressBook::Entry person2 = results[1];

	// Remove first two entries in results
	ab.remove(person1);
	ab.remove(person2);

	// Sort by last names
	std::vector<AddressBook::Entry> resultsRemoved = ab.sortedByLastName();

	// There should now only be 4 entries in the address book. having removed two entrries
	ASSERT_EQ(resultsRemoved.size(), 4);

	// Attempting to retrieve entries of persons removed
	std::vector<AddressBook::Entry> query1 = ab.find(person1.first_name);
	std::vector<AddressBook::Entry> query2 = ab.find(person2.first_name);

	// There should be 0 entries retrieved for each query
	ASSERT_EQ(query1.size(), 0);
	ASSERT_EQ(query2.size(), 0);

}



int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}