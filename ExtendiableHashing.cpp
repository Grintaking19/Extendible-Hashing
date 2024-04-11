
#include "ExtendiableHashing.h"
#include <bitset>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

void displayNotFound(int key)
{
	std::cout << "\t\t\t key:  " << key << " \t value : \t not found" << std::endl;
}

// Display one record entry, please don't change this function
void displayItem(DataItem *dataItem)
{
	if (dataItem != 0 && dataItem->valid)
	{
		std::cout << "\t\t\t key: " << std::bitset<8>(dataItem->key) << "\t value:\t" << dataItem->data << std::endl;
	}
	else
	{
		std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
	}
}

void displayBucket(Bucket &currentBucket, string &depths, string &values, int verbose)
{
	/*
		This function displays the bucket, it takes the following
		inputs:
		currentBucket: the bucket to be displayed
		depths: string to be appended with the local depth of the bucket
		values: string to be appended with the values of the bucket
		verbose: if 1, it will print the bucket content, otherwise it will not
	*/
	depths.append(std::to_string(currentBucket.localDepth));
	depths.append(",");
	values.append("[");
	if (verbose)
		std::cout << "\t\tBucket:\t local depth:" << currentBucket.localDepth << std::endl;
	std::flush(std::cout);
	if (currentBucket.currentEntries == 0)
	{
		if (verbose)
			std::cout << "\t\t\tNo Data yet\n";
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			values.append("null");
			values.append(",");
		}
	}
	else
	{
		for (int i = 0; i < RECORDSPERBUCKET; i++)
		{
			if (currentBucket.dataItem[i].valid)
			{
				values.append(std::to_string(currentBucket.dataItem[i].data));
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key: " << std::bitset<8>(currentBucket.dataItem[i].key) << "\t value:\t" << currentBucket.dataItem[i].data << std::endl;
				std::flush(std::cout);
			}
			else
			{
				values.append("null");
				values.append(",");
				if (verbose)
					std::cout << "\t\t\t key:  ============ \t value:\t empty data" << std::endl;
				std::flush(std::cout);
			}
		}
	}
	values.pop_back();	// remove last comma
	values.append("]"); // close the bracket
}

void displayDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket, int verbose)
{
	std::cout << "Directory:\t global depth:" << globaldirectory.globalDepth << std::endl;
	std::flush(std::cout);
	string values = "(";
	string depths = "(";
	int count = 0;
	// string locations = "(";
	if (globaldirectory.length == 0)
	{
		count++; // consider the empty directory as one bucket
		std::cout << "\tNo Directory yet\n";
		displayBucket(currentBucket, depths, values, verbose);
		std::flush(std::cout);
	}
	else
	{

		for (int i = 0; i < globaldirectory.length; i++)
		{
			if (i == 0)
			{
				count++;
			}
			else
			{
				if (globaldirectory.entry[i - 1] != globaldirectory.entry[i]) // if the pointer is not the same as the previous one, then it is a new bucket
					count++;
			}
			if (verbose)
				std::cout << "\t key: " << std::bitset<8>(i) << "\t value:\t" << globaldirectory.entry[i] << std::endl;
			displayBucket(*globaldirectory.entry[i], depths, values, verbose);
			if (verbose)
				std::cout << "-----------------------------------------------\n\n";
		}
		// values.pop_back();
		depths.pop_back();
	}

	values.append(")");
	depths.append(")");
	std::cout << " buckets:\t" << count << "/" << globaldirectory.length << endl;
	std::flush(std::cout);
	std::cout << "values:\t" << values << endl;
	std::flush(std::cout);
	std::cout << "depths:\t" << depths << endl;
	std::flush(std::cout);
	std::cout << "=========================\n";
	// std::cout << "Press any key to continue\n";
	// char t[100];
	// std::cin >> t;
}

// Hashing function and getting directory Index, please don't change this function,
int getCurrentHash(int key, int depth)
{
	// int hashedKey = (key & MAXKEYVALUE) >> (MAXKEYLENGTH - depth); // work on most significant bits
	// hash the key such that remaining depth bits from the LSB are taken
	// for example if depth = 3, then the last 3 bits from the key are taken
	int hashedKey = key & ((1 << depth) - 1);
	return hashedKey;
}

// TODO1: Implement this function, Don't change the interface please
//  functionlity: try to insert item into a bucket
//  return:   1 if succedded
//			 0 if failed
//  input:   currentBucket, dataItem to be inserted (check ExtendiableHashing.h for the content of the file)
//  Hint1: don't forget to update currentEntries upon insertion, you will need it later

int insertItemIntoBucket(Bucket &currentBucket, DataItem data)
{
	int inserted = 0;
	if (data.key > MAXKEYVALUE || data.key < 0) // invalid key
	{
		return inserted; // 0
	}

	if (currentBucket.currentEntries < RECORDSPERBUCKET)
	{
		currentBucket.dataItem[currentBucket.currentEntries] = data;
		currentBucket.currentEntries += 1;
		inserted = 1;
	}
	return inserted;
}

// TODO2: Implement this function, Don't change the interface please
//  functionlity: look for an item in a bucket using key, if found call displayItem(..), if not found call displayNotFound()
//  return:   nothing
//  input:   currentBucket, key value to search for
//  Hint: use displayNotFound & displayItem functions

void findItemInBucket(Bucket &currentBucket, int key)
{

	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (currentBucket.dataItem[i].key == key && currentBucket.dataItem[i].valid == 1)
		{
			displayItem(&currentBucket.dataItem[i]);
			return;
		}
	}
	displayNotFound(key);
}

// TODO3: Implement this function, Don't change the interface please
//  functionlity: try to Delete item based on a key value from a bucket
//  return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
//  input:   currentBucket, key to be inserted (check ExtendiableHashing.h)
//  Hint1:   don't forget to update currentEntries upon deletion, you will need it later

int deleteItemFromBucket(Bucket &currentBucket, int key)
{
	int deleted = 0;
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		if (currentBucket.dataItem[i].key == key && currentBucket.dataItem[i].valid == 1)
		{
			currentBucket.dataItem[i].valid = 0;
			deleted = 1;
			currentBucket.currentEntries -= 1;
			break;
		}
	}
	return deleted;
}

// TODO4: Implement this function, Don't change the interface please
//  functionlity: try to insert item in the file, if the bucket is full, extend the directory,
//				  if you extended the directory five times but it still doesn't work, return 0
//  return:   1 if succedded
//			 0 if failed (when does it fail to insert??)
//  input:   data:--> dataitem to be inserted, currentBucket --> initialBucket before creating director , globaldirectory
//  Hint: use insertItemIntoBucket,getCurrentHash, extendDirectory functions
//  Hint1:   don't forget to check for corner cases, for example if several entries points to the same bucket and you are going to split it. --> when local_depth = global_depth
//  Hint2:   a bucket could be split without expanding the directory (think when this will happen?) --> when local depth < global depth
//  Hint3:   don't forget to delete extra data --> After splitting the bucket, you redistribute the data over the two buckets. (May lead to data deleted and inserte in the another bucket)

int insertItem(DataItem data, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{

	// First corner case is done for you: if no directory yet
	if (globaldirectory.globalDepth == 0)
	{
		if (insertItemIntoBucket(currentBucket, data) == 0) // if the intial bucket is full, create the first directory of depth 1
		{
			createFirstTimeDirectory(globaldirectory, currentBucket);
		}
		else
		{
			return 1; // successfully inserted;
		}
	}
	// complete the rest of the function ....
	int hashedKey = getCurrentHash(data.key, globaldirectory.globalDepth);
	if (hashedKey == -1) // invalid key
	{
		return 0;
	}
	if (insertItemIntoBucket(*globaldirectory.entry[hashedKey], data) == 0) // the bucket is full
	{
		/*
			bucket_local_depth == directory_global_depth  --> expand the directory
			bucket_local_depth < directory_global_depth   --> split the bucket
		*/
		if (globaldirectory.entry[hashedKey]->localDepth == globaldirectory.globalDepth)
		{
			// expand the directory until you can insert the data
			while (insertItemIntoBucket(*globaldirectory.entry[hashedKey], data) == 0)
			{
				if (extendDirectory(globaldirectory, hashedKey) == 0)
				{
					return 0;
				}
				hashedKey = getCurrentHash(data.key, globaldirectory.globalDepth); // rehash the data
			}
		}
		else if (globaldirectory.entry[hashedKey]->localDepth < globaldirectory.globalDepth)
		{
			// Note that hashing is done based on local depth here
			// While splitting in the extendDirectory function is done based on global depth (local depth = global depth)
			globaldirectory.entry[hashedKey]->localDepth += 1;
			// rehash the data
			hashedKey = getCurrentHash(data.key, globaldirectory.entry[hashedKey]->localDepth);
			globaldirectory.entry[hashedKey] = new Bucket(globaldirectory.entry[hashedKey]->localDepth);
			// reditribute the data over the two buckets
			DataItem arr[2] = {globaldirectory.entry[hashedKey - globaldirectory.length / 2]->dataItem[0], globaldirectory.entry[hashedKey - globaldirectory.length / 2]->dataItem[1]};
			globaldirectory.entry[hashedKey - globaldirectory.length / 2]->dataItem[0].valid = 0;
			globaldirectory.entry[hashedKey - globaldirectory.length / 2]->dataItem[1].valid = 0;
			globaldirectory.entry[hashedKey - globaldirectory.length / 2]->currentEntries = 0;
			for (int j = 0; j < RECORDSPERBUCKET; j++)
			{
				int newKey = getCurrentHash(globaldirectory.entry[hashedKey - globaldirectory.length / 2]->dataItem[j].key, globaldirectory.entry[hashedKey]->localDepth); // get the new key for the DataItem
				insertItemIntoBucket(*globaldirectory.entry[newKey], arr[j]);																																															 // insert the DataItem in the new bucket
			}
			// insert the new item
			hashedKey = getCurrentHash(data.key, globaldirectory.entry[hashedKey]->localDepth);
			insertItemIntoBucket(*globaldirectory.entry[hashedKey], data);
		}
	}
	return 1;
}

// TODO5: Implement this function, Don't change the interface please
//  functionlity: search the directory for an item using the key
//  return:   nothing
//  input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
//  Hint1:   use findItemInBucket & getCurrentHash functions

void searchItem(int key, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	int hashedKey = getCurrentHash(key, globaldirectory.globalDepth);
	if (hashedKey == -1)
	{
		displayNotFound(key);
		return;
	}
	if (globaldirectory.globalDepth == 0)
	{
		findItemInBucket(currentBucket, key);
		return;
	}
	findItemInBucket(*globaldirectory.entry[hashedKey], key);
}

// TODO6: Implement this function, Don't change the interface please
//  functionlity: search on an item based on the key and delete it.
//  return:   1 if succedded
//			 0 if failed (when does it fail to delete??)
//  input:   key to be searched for , currentBucket --> initialBucket before creating directory,  globaldirectory
//  Hint:    use deleteItemFromBucket & getCurrentHash & checkDirectoryMinimization functions
//  Hint1:   in case the whole bucket is empty, the bucket should be merged again and the pointer should point to the peer bucket
//  Hint2:   in how many steps do we reach our goal?
//  Hint3:   in case of delete success don't forget to call checkDirectoryMinimization to compress directory if needed.
//  Hint4: You might want to loop on checkDirectoryMinimization, not just call it once to continue merging

int deleteItem(int key, Bucket &currentBucket, GlobalDirectory &globaldirectory)
{
	if (globaldirectory.globalDepth == 0)
	{
		if (deleteItemFromBucket(currentBucket, key) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	int hashedKey = getCurrentHash(key, globaldirectory.globalDepth);
	if (hashedKey == -1)
	{
		return 0;
	}
	if (deleteItemFromBucket(*globaldirectory.entry[hashedKey], key) == 0)
	{
		return 0;
	}
	if (globaldirectory.entry[hashedKey]->currentEntries == 0)
	{

		// there are many cases, these includes if the bucket has local depth = global depth
		// or if the bucket has local depth < global depth
		if (globaldirectory.entry[hashedKey]->localDepth == globaldirectory.globalDepth)
		{
			// these means only one bucket points to this bucket, so we can delete it
			delete globaldirectory.entry[hashedKey];
			if (hashedKey < globaldirectory.length / 2)
			{
				globaldirectory.entry[hashedKey] = globaldirectory.entry[hashedKey + globaldirectory.length / 2];
			}
			else
			{
				globaldirectory.entry[hashedKey] = globaldirectory.entry[hashedKey - globaldirectory.length / 2];
			}
		}
		else if (globaldirectory.entry[hashedKey]->localDepth < globaldirectory.globalDepth)
		{
			// these means more than one bucket pointer points to this bucket, so we have to traverse the directory to find other pointers
			// and make them point to the peer bucket
			int newHash = getCurrentHash(key, globaldirectory.entry[hashedKey]->localDepth -1);
			Bucket *emptyBucket = globaldirectory.entry[hashedKey];
			for (int i = 0; i < globaldirectory.length; i++)
			{
				// if the pointer points to entry[hashedKey], make it point to newHash entry 
				if (globaldirectory.entry[i] == emptyBucket)
				{
					globaldirectory.entry[i] = globaldirectory.entry[newHash];
				}
				
			}
			delete emptyBucket;
		}
		// globaldirectory.entry[hashedKey] = globaldirectory.entry[hashedKey - globaldirectory.length / 2];
		// decrease the local depth
		globaldirectory.entry[hashedKey]->localDepth -= 1;
		// check if we can minimize the directory --> it will be minimized if all buckets have depth less than global depth
		while(checkDirectoryMinimization(globaldirectory) == 1); // loop on checkDirectoryMinimization until it returns -1
		return 1;
	}
	else
	{
		return 1;
	}
	return 0;
}

// create  the first directory, this might help you to implement extendDirectory
int createFirstTimeDirectory(GlobalDirectory &globaldirectory, Bucket &currentBucket)
{
	globaldirectory.globalDepth = 1;
	globaldirectory.length = 2; // 2^globalDepth
	globaldirectory.entry = new Bucket *[globaldirectory.length];
	globaldirectory.entry[0] = new Bucket(globaldirectory.globalDepth);
	globaldirectory.entry[1] = new Bucket(globaldirectory.globalDepth);
	for (int i = 0; i < RECORDSPERBUCKET; i++)
	{
		int newKey = getCurrentHash(currentBucket.dataItem[i].key, globaldirectory.globalDepth);
		if (newKey == -1) // this happens when the key is not valid (i.e. empty)
		{
			return -1;
		}
		insertItemIntoBucket(*globaldirectory.entry[newKey], currentBucket.dataItem[i]);
	}
	return 1;
}

// TODO7: Implement this function, Don't change the interface please
//  functionlity: this expands the directory because we can't find a space anymore in the file,
//                it also redistrubtes data over the split buckets
//  return:   1 if succedded
//			 0 if failed (when does it fail to expand??)
//  input:   Directory, hashKey(bucket index) at which the overflow occurs
//  Hint1:   don't forget todelete unneeded pointers to avoid memory leakage
//  Hint2:   what is the size of the new directory compared to old one? what is the new depth?
//  Hint3:   some entries will point to the same bucket
int extendDirectory(GlobalDirectory &globaldirectory, int splitIndex)
{

	if (globaldirectory.globalDepth == MAXKEYLENGTH) // we can't expand more
		return 0;
	globaldirectory.globalDepth += 1;																					// increase the global depth
	globaldirectory.length *= 2;																							// double the length
	Bucket **oldEntry = globaldirectory.entry;																	// save the old entry
	globaldirectory.entry = new Bucket *[globaldirectory.length];							// create a new entry
	for (int i = 0; i < globaldirectory.length / 2; i++)												// iterate over the old half of the directory
	{
		globaldirectory.entry[i] = oldEntry[i];																	// make the new pointers point to the same buckets as the old ones
	}
	delete[] oldEntry;																												// delete the old entry
	for (int i = globaldirectory.length / 2; i < globaldirectory.length; i++) // iterate over the new half of the directory
	{
		if (i != splitIndex + globaldirectory.length / 2)
		{
			globaldirectory.entry[i] = globaldirectory.entry[i - globaldirectory.length / 2]; // make the new pointers point to the same buckets as the old ones
		}
		else
		{
			globaldirectory.entry[i] = new Bucket(globaldirectory.globalDepth); // create a new bucket for the split index
			// redistribute the data over the two buckets (entry[splitIndex] and entry[splitIndex + globaldirectory.length/2])
			DataItem arr[2] = {globaldirectory.entry[splitIndex]->dataItem[0], globaldirectory.entry[splitIndex]->dataItem[1]};
			globaldirectory.entry[i - globaldirectory.length / 2]->dataItem[0].valid = 0;
			globaldirectory.entry[i - globaldirectory.length / 2]->dataItem[1].valid = 0;
			globaldirectory.entry[i - globaldirectory.length / 2]->currentEntries = 0;
			globaldirectory.entry[i - globaldirectory.length / 2]->localDepth += 1;
			for (int j = 0; j < RECORDSPERBUCKET; j++)
			{

				int newKey = getCurrentHash(globaldirectory.entry[splitIndex]->dataItem[j].key, globaldirectory.globalDepth); // get the new key for the DataItem
				insertItemIntoBucket(*globaldirectory.entry[newKey], arr[j]);																									// insert the DataItem in the new bucket
			}
		}
	}
	return 1;
}

// If all buckets have depth less than global depth,
//  compress the directory depth by one
//  loop on this function as appropriate to compress the file back
int checkDirectoryMinimization(GlobalDirectory &globaldirectory)
{

	for (int i = 0; i < globaldirectory.length; i++)
	{
		if (globaldirectory.entry[i]->localDepth == globaldirectory.globalDepth)
			return -1;
	}
	// if its depth =1, there is nothing more we can do, stop here
	if (globaldirectory.length == 1)
		return -1;
	// all are smaller than localdepth, initiate minimization
	//  this is actually too stupid, it should be at least half empty to initiate minimization
	//  but we will keep it like that for simplicity
	int oldGlobalDepth, oldLength;
	Bucket **oldEntry;

	oldGlobalDepth = globaldirectory.globalDepth;
	oldLength = globaldirectory.length;
	globaldirectory.globalDepth -= 1;
	globaldirectory.length /= 2;
	oldEntry = globaldirectory.entry;
	globaldirectory.entry = new Bucket *[globaldirectory.length];
	for (int i = 0; i < globaldirectory.length; i++)
	{
		globaldirectory.entry[i] = oldEntry[2 * i];
	}

	delete[] oldEntry;
	// delete oldEntry;
	return 1;
}