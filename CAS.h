#include <atomic>



class CAS {
public:
	CAS() = default;

	CAS(CAS& other) = delete;

	CAS& operator=(const CAS&) = delete;
	void lock()
	{
		bool expect = false;
		while (!locker.compare_exchange_strong(expect, true))
		{
			expect = false;
		}
	}
	void unlock()
	{
		locker.store(false);
	}

private:
	std::atomic<bool> locker;
};