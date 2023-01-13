/***************************************************************************
 *   Free Heroes of Might and Magic II: https://github.com/ihhub/fheroes2  *
 *   Copyright (C) 2021                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "timing.h"

#include <thread>


CTime::CTime()
    : _startTime( std::chrono::high_resolution_clock::now() )
{}

void CTime::reset() {
    _startTime = std::chrono::high_resolution_clock::now();
}

double CTime::get() const {
    const std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - _startTime;
    return time.count();
}

uint64_t CTime::getMs() const {
    return static_cast<uint64_t>( get() * 1000 + 0.5 );
}

CTimeDelay::CTimeDelay( const uint64_t delayMs )
    : _prevTime( std::chrono::high_resolution_clock::now() )
    , _delayMs( delayMs )
{}

void CTimeDelay::setDelay( const uint64_t delayMs ) {
    _delayMs = delayMs;
}

bool CTimeDelay::isPassed() const {
    return isPassed( _delayMs );
}

bool CTimeDelay::isPassed( const uint64_t delayMs ) const {
    const std::chrono::duration<double> time = std::chrono::high_resolution_clock::now() - _prevTime;
    const uint64_t passedMs = static_cast<uint64_t>( time.count() * 1000 + 0.5 );
    return passedMs >= delayMs;
}

void CTimeDelay::reset() {
    _prevTime = std::chrono::high_resolution_clock::now();
}

void CTimeDelay::pass() {
    _prevTime = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds( 2 * _delayMs );
}

void delayforMs( const uint32_t delayMs ) {
    std::this_thread::sleep_for( std::chrono::milliseconds( delayMs ) );
}

