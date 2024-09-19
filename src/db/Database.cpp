#include <db/Database.hpp>

using namespace db;

BufferPool &Database::getBufferPool() { return bufferPool; }

Database &db::getDatabase() {
  static Database instance;
  return instance;
}

void Database::add(std::unique_ptr<DbFile> file) {
  // TODO pa1: add the file to the catalog. Note that the file must not exist.
  if (catalog.find(file->getName()) != catalog.end()) {
    throw std::logic_error("File already exists in the catalog.");
  }
  // Add the file to the catalog
  catalog[file->getName()] = std::move(file);  // Ownership is transferred
}

std::unique_ptr<DbFile> Database::remove(const std::string &name) {
  // TODO pa1: remove the file from the catalog. Note that the file must exist.
  auto it = catalog.find(name);
  if (it == catalog.end()) {
    throw std::logic_error("File does not exist in the catalog.");
  }

  // Before removing, flush the associated file's pages from the buffer pool
  bufferPool.flushFile(name);

  // Move the file out of the catalog
  std::unique_ptr<DbFile> removedFile = std::move(it->second);
  catalog.erase(it);  // Remove the file from the catalog

  return removedFile;
}

DbFile &Database::get(const std::string &name) const {
  // TODO pa1: get the file from the catalog. Note that the file must exist.
  // Find the file in the catalog
  auto it = catalog.find(name);
  if (it == catalog.end()) {
    throw std::logic_error("File does not exist in the catalog.");
  }

  // Return a reference to the found file
  return *(it->second);
}
