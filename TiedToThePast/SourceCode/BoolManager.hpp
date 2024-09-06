#ifndef BOOL_MANAGER__HPP
#define BOOL_MANAGER__HPP

class BoolManager
{
public:
	~BoolManager(void) = default;
	BoolManager(const BoolManager&) = delete;
	BoolManager(BoolManager&&) = delete;
	BoolManager& operator=(const BoolManager&) = delete;
	BoolManager& operator=(BoolManager&&) = delete;

	static BoolManager& Get(void);
	bool& operator[](const std::string& _key);

	void Load(std::ifstream& _buffer);
	void Save(std::ofstream& _buffer);
	void Reset(void);

private:
	BoolManager(void) = default;

protected:
	std::unordered_map<std::string, bool> boolMap;
};

#endif // !BOOL_MANAGER__HPP