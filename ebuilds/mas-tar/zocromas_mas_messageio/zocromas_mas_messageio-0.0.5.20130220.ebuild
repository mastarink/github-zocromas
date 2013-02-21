# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

AUTOTOOLS_AUTORECONF=1

DESCRIPTION="mastar tools library"
HOMEPAGE="http://zocromas.mastarink.net/zocromas/tools.html"
SRC_URI="http://zocromas.mastarink.net/zocromas/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~amd64"
IUSE=""

DEPEND="dev-libs/glib:2 mas-tar/zoctypes mas-tar/mas_wrap mas-tar/mas_msg mas-tar/mas_io mas-tar/mas_channel"
RDEPEND="${DEPEND}"

src_configure() {
    econf  --enable-silent-rules --enable-tracemem --enable-debug
}

src_install() {
    emake DESTDIR="${D}" install

#    dodoc FAQ NEWS README
#    dohtml EXTENDING.html ctags.html
}
