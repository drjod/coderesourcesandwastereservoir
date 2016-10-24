/////////////////////////////////////////////////////////////////////
// Hack at Global Day of Code Retreat 2016 - Hamburg 
// code by Gunther Bachman
//



package gol;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

public class PlaygroundTest {

	Playground classUnderTest;

	@Before
	public void setUp() {
		classUnderTest = new Playground(5, 5);
	}

	@Test
	public void testPlaygroundInitial() {
		assertFalse(classUnderTest.isAlive());
	}

	@Test
	public void testPlaygroundNotInitial() {
		// given
		classUnderTest.setCell(1, 1, 1);

		// when, then
		assertTrue(classUnderTest.isAlive());
	}

	@Test
	public void testSetGet() {
		// given
		// when
		classUnderTest.setCell(1, 1, 1);

		// then
		assertEquals(1, classUnderTest.getCell(1, 1));
	}

	@Test
	public void testNewStatus() {
		// given
		classUnderTest.initField(//
				"     ", //
				"  *  ", //
				"  *  ", //
				"  *  ", //
				"     ");
		
		// when, then
		assertEquals(1, classUnderTest.determineNewCellStatus(2, 2));
		assertEquals(0, classUnderTest.determineNewCellStatus(2, 1));
		assertEquals(0, classUnderTest.determineNewCellStatus(2, 3));
		assertEquals(1, classUnderTest.determineNewCellStatus(1, 2));
		assertEquals(1, classUnderTest.determineNewCellStatus(3, 2));
		assertEquals(0, classUnderTest.determineNewCellStatus(1, 1));
	}
}
