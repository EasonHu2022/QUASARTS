#pragma once

#include "Core/Core.h"


namespace Engine
{
	class QS_API QTime
	{
	public:
		QTime() : mTsec(0) {}
		~QTime() {}

		QTime(double aTimeSeconds) : mTsec(aTimeSeconds) {}


		/// <summary>
		/// Get value of time in seconds.
		/// </summary>
		/// <returns>T (s)</returns>
		double sec() { return mTsec; }

		/// <summary>
		/// Get value of time in milliseconds.
		/// Testing shows no difference between manual conversion
		/// and duration_cast().
		/// </summary>
		/// <returns>T (ms)</returns>
		float ms() { return double(mTsec * 1e3); }


	private:
		double mTsec;


		// Operators //
	public:
		QTime operator+(QTime const& rhs) const {
			return QTime(mTsec + rhs.mTsec);
		}
		QTime operator-(QTime const& rhs) const {
			return QTime(mTsec - rhs.mTsec);
		}
		QTime& operator=(QTime const& rhs) {
			mTsec = rhs.mTsec;
			return *this;
		}
		QTime& operator+=(QTime const& rhs) {
			mTsec += rhs.mTsec;
			return *this;
		}
		QTime& operator-=(QTime const& rhs) {
			mTsec -= rhs.mTsec;
			return *this;
		}
		bool operator==(QTime const& rhs) {
			return (mTsec == rhs.mTsec);
		}
		bool operator<(QTime const& rhs) {
			return (mTsec < rhs.mTsec);
		}
		bool operator>(QTime const& rhs) {
			return (mTsec > rhs.mTsec);
		}
	};
}